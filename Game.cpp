#include "Game.hpp"
#include <algorithm>
#include <optional>

using namespace sf;
using namespace std;

namespace {
    const unsigned int WINDOW_WIDTH  = 960;
    const unsigned int WINDOW_HEIGHT = 540;
    const float W = static_cast<float>(WINDOW_WIDTH);
    const float H = static_cast<float>(WINDOW_HEIGHT);

    const char* MENU_ITEMS[] = { "Play", "High Scores", "Quit" };
    const int MENU_COUNT = 3;

    // Fonts tried in order: one shipped with the project first, then Windows' Arial
    const char* FONT_PATHS[] = {
        "assets/fonts/font.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf"
    };
}


Game::Game()
    : window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "OOP Platformer"),
      view(FloatRect({0.f, 0.f}, {W, H})),
      state(GameState::MENU),
      player(100.f, 300.f),
      exitBounds({0.f, 0.f}, {0.f, 0.f}),
      levelWidth(W),
      levelHeight(H),
      fontLoaded(false),
      currentLevel(1),
      totalLevels(2),
      menuIndex(0) {
    window.setFramerateLimit(60);

    for (const char* path : FONT_PATHS) {
        if (font.openFromFile(path)) {
            fontLoaded = true;
            break;
        }
    }
}


void Game::run() {
    Clock clock;
    while (window.isOpen()) {
        // Cap the timestep so a lag spike can't make the player fall through platforms
        float deltaTime = min(clock.restart().asSeconds(), 1.f / 30.f);
        processEvents();
        update(deltaTime);
        render();
    }
}


// ---------------------------------------------------------------- input

void Game::processEvents() {
    while (const optional<Event> event = window.pollEvent()) {
        if (event->is<Event::Closed>()) {
            window.close();
            return;
        }

        const auto* key = event->getIf<Event::KeyPressed>();
        if (!key)
            continue;

        switch (state) {
            case GameState::MENU:
                if (key->code == Keyboard::Key::Up || key->code == Keyboard::Key::W)
                    menuIndex = (menuIndex + MENU_COUNT - 1) % MENU_COUNT;
                else if (key->code == Keyboard::Key::Down || key->code == Keyboard::Key::S)
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                else if (key->code == Keyboard::Key::Enter) {
                    if (menuIndex == 0)      startNewRun();
                    else if (menuIndex == 1) state = GameState::HIGH_SCORES;
                    else                     window.close();
                }
                else if (key->code == Keyboard::Key::Escape)
                    window.close();
                break;

            case GameState::PLAYING:
                if (key->code == Keyboard::Key::Escape || key->code == Keyboard::Key::P)
                    state = GameState::PAUSED;
                break;

            case GameState::PAUSED:
                if (key->code == Keyboard::Key::P || key->code == Keyboard::Key::Enter)
                    state = GameState::PLAYING;
                else if (key->code == Keyboard::Key::Escape)
                    state = GameState::MENU;
                break;

            case GameState::HIGH_SCORES:
                if (key->code == Keyboard::Key::Escape || key->code == Keyboard::Key::Enter)
                    state = GameState::MENU;
                break;

            case GameState::VICTORY:
            case GameState::GAME_OVER:
                if (key->code == Keyboard::Key::Enter || key->code == Keyboard::Key::Escape)
                    state = GameState::MENU;
                break;
        }
    }
}


// ---------------------------------------------------------------- game logic

void Game::update(float deltaTime) {
    if (state != GameState::PLAYING)
        return;

    player.handleInput();
    player.update(deltaTime);

    for (Platform& p : platforms) {
        p.update(deltaTime);
        player.resolveCollision(p);
    }

    // Fell off the bottom of the level
    if (player.getBounds().position.y > levelHeight + 100.f) {
        if (player.isInvincible())
            player.respawn();
        else
            player.takeDamage();
    }

    if (!player.isAlive()) {
        endRun(false);
        return;
    }

    if (player.intersects(exitBounds)) {
        advanceLevel();
        return;
    }

    // Camera follows the player but stays inside the level
    FloatRect pb = player.getBounds();
    float camX = pb.position.x + pb.size.x / 2.f;
    camX = max(W / 2.f, min(camX, levelWidth - W / 2.f));
    view.setCenter({camX, H / 2.f});
}


void Game::startNewRun() {
    currentLevel = 1;
    loadLevel(currentLevel);
    player.resetRun();
    state = GameState::PLAYING;
}


// Hard-coded test levels. TODO: replace with Level loading from a file.
void Game::loadLevel(int number) {
    platforms.clear();
    const float ground = H - 40.f;

    if (number == 1) {
        levelWidth = 2400.f;
        platforms.emplace_back(0.f, ground, levelWidth, 40.f);         // one continuous floor

        platforms.emplace_back(300.f,  ground - 110.f, 160.f, 20.f);
        platforms.emplace_back(550.f,  ground - 200.f, 160.f, 20.f);
        platforms.emplace_back(850.f,  ground - 130.f, 200.f, 20.f);
        platforms.emplace_back(1250.f, ground - 150.f, 180.f, 20.f);
        platforms.emplace_back(1500.f, ground - 250.f, 140.f, 20.f);

        exitBounds = FloatRect({2300.f, ground - 80.f}, {40.f, 80.f});
        player.setSpawn(80.f, ground - 100.f);
    } else {
        levelWidth = 2800.f;
        platforms.emplace_back(0.f, ground, levelWidth, 40.f);         // one continuous floor

        platforms.emplace_back(650.f,  ground - 80.f,  140.f, 20.f);
        platforms.emplace_back(900.f,  ground - 170.f, 140.f, 20.f);
        platforms.emplace_back(1150.f, ground - 90.f,  140.f, 20.f);
        platforms.emplace_back(1900.f, ground - 140.f, 120.f, 20.f);
        platforms.emplace_back(2150.f, ground - 230.f, 120.f, 20.f);

        exitBounds = FloatRect({2700.f, ground - 80.f}, {40.f, 80.f});
        player.setSpawn(80.f, ground - 100.f);
    }

    player.respawn();
    view.setCenter({W / 2.f, H / 2.f});
}


void Game::advanceLevel() {
    player += 500;   // level-complete bonus
    if (currentLevel < totalLevels) {
        currentLevel++;
        loadLevel(currentLevel);
    } else {
        endRun(true);
    }
}


void Game::endRun(bool won) {
    // TODO: scoreBoard.addScore(name, player.getScore()); scoreBoard.save();
    state = won ? GameState::VICTORY : GameState::GAME_OVER;
}


// ---------------------------------------------------------------- drawing

void Game::render() {
    window.clear(Color(135, 206, 235));   // sky blue

    switch (state) {
        case GameState::MENU:
            window.setView(window.getDefaultView());
            drawMenu();
            break;

        case GameState::HIGH_SCORES:
            window.setView(window.getDefaultView());
            drawHighScores();
            break;

        case GameState::PLAYING:
        case GameState::PAUSED:
        case GameState::VICTORY:
        case GameState::GAME_OVER: {
            // World (scrolls with the camera)
            window.setView(view);
            for (Platform& p : platforms)
                p.draw(window);

            // Exit flag
            RectangleShape pole({6.f, exitBounds.size.y});
            pole.setPosition(exitBounds.position);
            pole.setFillColor(Color(80, 80, 80));
            window.draw(pole);
            RectangleShape flag({32.f, 22.f});
            flag.setPosition({exitBounds.position.x + 6.f, exitBounds.position.y});
            flag.setFillColor(Color(230, 57, 70));
            window.draw(flag);

            player.draw(window);

            // UI (fixed on screen)
            window.setView(window.getDefaultView());
            drawHUD();

            if (state == GameState::PAUSED)    drawOverlay("Paused");
            if (state == GameState::VICTORY)   drawOverlay("You Win!");
            if (state == GameState::GAME_OVER) drawOverlay("Game Over");
            break;
        }
    }

    window.display();
}


void Game::drawText(const string& str, unsigned int size, Vector2f pos,
                    Color color, bool centered) {
    if (!fontLoaded)
        return;

    Text text(font, str, size);
    text.setFillColor(color);
    text.setOutlineColor(Color(0, 0, 0, 160));
    text.setOutlineThickness(2.f);

    if (centered) {
        FloatRect b = text.getLocalBounds();
        text.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
    }
    text.setPosition(pos);
    window.draw(text);
}


void Game::drawMenu() {
    // Decorative ground strip
    RectangleShape ground({W, 60.f});
    ground.setPosition({0.f, H - 60.f});
    ground.setFillColor(Color(76, 175, 80));
    window.draw(ground);

    drawText("OOP PLATFORMER", 64, {W / 2.f, 120.f}, Color::White);

    for (int i = 0; i < MENU_COUNT; i++) {
        bool selected = (i == menuIndex);
        float itemY = 250.f + i * 60.f;

        if (selected) {
            RectangleShape highlight({260.f, 48.f});
            highlight.setOrigin({130.f, 24.f});
            highlight.setPosition({W / 2.f, itemY});
            highlight.setFillColor(Color(0, 0, 0, 90));
            window.draw(highlight);
        }
        drawText(MENU_ITEMS[i], selected ? 36 : 30, {W / 2.f, itemY},
                 selected ? Color(255, 215, 0) : Color::White);
    }

    drawText("Up/Down to choose, Enter to select", 18, {W / 2.f, H - 30.f}, Color::White);

    if (!fontLoaded) {
        // No font found: at least show the selection as boxes so the menu is usable
        for (int i = 0; i < MENU_COUNT; i++) {
            RectangleShape box({200.f, 40.f});
            box.setOrigin({100.f, 20.f});
            box.setPosition({W / 2.f, 250.f + i * 60.f});
            box.setFillColor(i == menuIndex ? Color(255, 215, 0) : Color(255, 255, 255, 120));
            window.draw(box);
        }
    }
}


void Game::drawHighScores() {
    drawText("HIGH SCORES", 56, {W / 2.f, 100.f}, Color::White);
    // TODO: list scoreBoard.getRecords() here once ScoreBoard is implemented
    drawText("No scores yet", 28, {W / 2.f, H / 2.f}, Color(240, 240, 240));
    drawText("Press Esc to go back", 18, {W / 2.f, H - 40.f}, Color::White);
}


void Game::drawHUD() {
    // Translucent bar across the top
    RectangleShape bar({W, 40.f});
    bar.setFillColor(Color(0, 0, 0, 110));
    window.draw(bar);

    drawText("Score: " + to_string(player.getScore()), 22, {16.f, 8.f}, Color::White, false);
    drawText("Level " + to_string(currentLevel) + " / " + to_string(totalLevels),
             22, {W / 2.f, 20.f}, Color::White);

    // Lives as hearts (red squares) on the right
    for (int i = 0; i < player.getLives(); i++) {
        RectangleShape heart({20.f, 20.f});
        heart.setPosition({W - 34.f - i * 28.f, 10.f});
        heart.setFillColor(Color(230, 57, 70));
        heart.setOutlineColor(Color::White);
        heart.setOutlineThickness(2.f);
        window.draw(heart);
    }
}


void Game::drawOverlay(const string& title) {
    RectangleShape dim({W, H});
    dim.setFillColor(Color(0, 0, 0, 150));
    window.draw(dim);

    drawText(title, 64, {W / 2.f, H / 2.f - 50.f}, Color::White);

    if (state == GameState::PAUSED) {
        drawText("P to resume, Esc for menu", 24, {W / 2.f, H / 2.f + 30.f}, Color::White);
    } else {
        drawText("Final score: " + to_string(player.getScore()), 28,
                 {W / 2.f, H / 2.f + 20.f}, Color(255, 215, 0));
        drawText("Press Enter to return to the menu", 22,
                 {W / 2.f, H / 2.f + 70.f}, Color::White);
    }
}
