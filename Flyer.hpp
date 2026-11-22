#ifndef FLYER_HPP
#define FLYER_HPP
#include <SFML/Graphics.hpp>
#include "Enemy.hpp"

using namespace sf;


class Flyer : public Enemy {
    private:
        float baseY;
        float amplitude;
        float frequency;
        float elapsed;

    public:
        Flyer(float x, float y);
        void update(float deltaTime) override;
};

#endif
