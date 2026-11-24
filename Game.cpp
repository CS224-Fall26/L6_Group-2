#include "Game.hpp"
#include <optional>

using namespace sf;
using namespace std;


Game::Game()
    : window(VideoMode({960, 540}), "Energy Dash"),
      player(450.f, 200.f),
      platform(180.f, 420.f, 600.f, 40.f) {
    window.setFramerateLimit(60);
}


void Game::run() {
    Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}


void Game::processEvents() {
    while (const optional<Event> event = window.pollEvent()) {
        if (event->is<Event::Closed>())
            window.close();
    }
}


void Game::update(float deltaTime) {
    player.handleInput();
    player.update(deltaTime);
    player.resolveCollision(platform);

    if (player.getBounds().position.y > 600.f)
        player.respawn();
}


void Game::render() {
    window.clear(Color(135, 206, 235));
    platform.draw(window);
    player.draw(window);
    window.display();
}
