#ifndef COIN_HPP
#define COIN_HPP
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

using namespace sf;


class Coin : public GameObject {
    private:
        int value;
        bool collected;

    public:
        Coin(float x, float y, int value);
        void update(float deltaTime) override;
        void draw(RenderWindow &window) override;
        int collect();
        bool isCollected() const;
};

#endif
