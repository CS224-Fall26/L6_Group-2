#ifndef ENEMY_HPP
#define ENEMY_HPP
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

using namespace sf;


class Enemy : public GameObject {
    protected:
        float speed;
        int direction;
        float minX;
        float maxX;
        bool alive;
        void patrolHorizontally(float deltaTime);

    public:
        Enemy(float x, float y);
        void update(float deltaTime) override = 0;
        void draw(RenderWindow &window) override;
        void setPatrolRange(float minX, float maxX);
        void defeat();
        bool isAlive() const;
};

#endif
