#ifndef PLATFORM_HPP
#define PLATFORM_HPP
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

using namespace sf;


class Platform : public GameObject {
    public:
        Platform(float x, float y, float w, float h);
        void update(float deltaTime) override;
        void draw(RenderWindow &window) override;
};

#endif
