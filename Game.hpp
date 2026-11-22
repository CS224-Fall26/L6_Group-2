#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "GameState.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "ScoreBoard.hpp"
#include "AssetManager.hpp"

using namespace sf;
using namespace std;


class Game {
    private:
        RenderWindow window;
        View view;
        GameState state;
        Player player;
        ScoreBoard scoreBoard;
        unique_ptr<Level> level;
        int currentLevel;
        int totalLevels;
        int menuIndex;
        void processEvents();
        void update(float deltaTime);
        void render();
        void startNewRun();
        void loadLevel(int number);
        void advanceLevel();
        void endRun(bool won);
        void drawMenu();
        void drawHighScores();
        void drawHUD();
        void drawOverlay(const string& title);

    public:
        Game();
        void run();
};

#endif
