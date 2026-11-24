#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include "Player.hpp"
#include "Platform.hpp"

using namespace sf;


class Game {
    private:
        RenderWindow window;
        Player player;
        Platform platform;
        void processEvents();
        void update(float deltaTime);
        void render();

    public:
        Game();
        void run();
};

#endif
