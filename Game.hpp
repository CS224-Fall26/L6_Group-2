#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "GameState.hpp"
#include "Player.hpp"
#include "Platform.hpp"
// TODO: re-enable once Level / ScoreBoard / AssetManager have .cpp files
// #include "Level.hpp"
// #include "ScoreBoard.hpp"
// #include "AssetManager.hpp"

using namespace sf;
using namespace std;


class Game {
    private:
        RenderWindow window;
        View view;
        GameState state;
        Player player;
        // ScoreBoard scoreBoard;      // TODO
        // unique_ptr<Level> level;    // TODO: replaces the fields below
        vector<Platform> platforms;    // temporary until Level is done
        FloatRect exitBounds;          // temporary until Level is done
        float levelWidth;              // temporary until Level is done
        float levelHeight;             // temporary until Level is done
        Font font;                     // temporary until AssetManager is done
        bool fontLoaded;
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
        void drawText(const string& str, unsigned int size, Vector2f pos,
                      Color color, bool centered = true);

    public:
        Game();
        void run();
};

#endif
