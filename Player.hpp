#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <SFML/Graphics.hpp>
#include <string>
#include "GameObject.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"
#include "Coin.hpp"

using namespace sf;
using namespace std;


class Player : public GameObject {
    private:
        string name;
        int lives;
        int score;
        float velocityX;
        float velocityY;
        float moveSpeed;
        float jumpStrength;
        bool onGround;
        float invincibleTimer;
        float spawnX;
        float spawnY;

    public:
        Player(float x, float y);
        void handleInput();
        void update(float deltaTime) override;
        void draw(RenderWindow &window) override;
        void jump();
        void resolveCollision(const Platform& p);
        void resolveEnemyCollision(Enemy& e);
        void bounce();
        void collectCoin(Coin& c);
        void takeDamage();
        void respawn();
        void setSpawn(float x, float y);
        bool isInvincible() const;
        bool isAlive() const;
        void resetRun();
        Player& operator+=(int points);
};

#endif
