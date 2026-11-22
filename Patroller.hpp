#ifndef PATROLLER_HPP
#define PATROLLER_HPP
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

using namespace sf;


class Patroller : public Enemy {
    public:
        Patroller(float x, float y);
        void update(float deltaTime) override;
};

#endif
