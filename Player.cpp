#include "Player.hpp"
#include <algorithm>

using namespace sf;
using namespace std;

namespace {
    const float GRAVITY = 1400.f;        // pixels per second^2
    const float MAX_FALL_SPEED = 900.f;
    const float INVINCIBLE_TIME = 2.f;   // seconds of invincibility after a hit
    const int   START_LIVES = 3;
    const int   STOMP_POINTS = 100;
}


Player::Player(float x, float y)
    : GameObject(x, y, 32.f, 48.f),
      name("Player"),
      lives(START_LIVES),
      score(0),
      velocityX(0.f),
      velocityY(0.f),
      moveSpeed(260.f),
      jumpStrength(620.f),
      onGround(false),
      invincibleTimer(0.f),
      spawnX(x),
      spawnY(y) {}


void Player::handleInput() {
    velocityX = 0.f;

    if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
        velocityX -= moveSpeed;
    if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
        velocityX += moveSpeed;

    if (Keyboard::isKeyPressed(Keyboard::Key::Space) ||
        Keyboard::isKeyPressed(Keyboard::Key::Up) ||
        Keyboard::isKeyPressed(Keyboard::Key::W))
        jump();
}


void Player::update(float deltaTime) {
    // Gravity
    velocityY += GRAVITY * deltaTime;
    velocityY = min(velocityY, MAX_FALL_SPEED);

    // Move
    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    // Don't walk off the left edge of the level
    if (x < 0.f) x = 0.f;

    // Becomes true again if resolveCollision() finds a platform underneath us
    onGround = false;

    if (invincibleTimer > 0.f)
        invincibleTimer -= deltaTime;
}


void Player::draw(RenderWindow &window) {
    // Blink while invincible
    if (isInvincible() && static_cast<int>(invincibleTimer * 10.f) % 2 == 0)
        return;

    RectangleShape body({width, height});
    body.setPosition({x, y});
    body.setFillColor(Color(66, 133, 244));
    body.setOutlineColor(Color(25, 60, 140));
    body.setOutlineThickness(-2.f);
    window.draw(body);

    // Eyes look in the direction we're moving
    float lookOffset = velocityX > 0.f ? 3.f : (velocityX < 0.f ? -3.f : 0.f);
    RectangleShape eye({6.f, 8.f});
    eye.setFillColor(Color::White);
    eye.setPosition({x + 8.f + lookOffset, y + 10.f});
    window.draw(eye);
    eye.setPosition({x + 18.f + lookOffset, y + 10.f});
    window.draw(eye);
}


void Player::jump() {
    if (onGround) {
        velocityY = -jumpStrength;
        onGround = false;
    }
}


// Push the player out of the platform along whichever axis has the smallest overlap
void Player::resolveCollision(const Platform& p) {
    FloatRect pb = p.getBounds();
    if (!intersects(pb))
        return;

    float overlapLeft   = (x + width)  - pb.position.x;
    float overlapRight  = (pb.position.x + pb.size.x) - x;
    float overlapTop    = (y + height) - pb.position.y;
    float overlapBottom = (pb.position.y + pb.size.y) - y;

    float minX = min(overlapLeft, overlapRight);
    float minY = min(overlapTop, overlapBottom);

    if (minY < minX) {
        if (overlapTop < overlapBottom) {
            // Landed on top
            y -= overlapTop;
            if (velocityY > 0.f) velocityY = 0.f;
            onGround = true;
        } else {
            // Hit head on the underside
            y += overlapBottom;
            if (velocityY < 0.f) velocityY = 0.f;
        }
    } else {
        if (overlapLeft < overlapRight)
            x -= overlapLeft;
        else
            x += overlapRight;
        velocityX = 0.f;
    }
}


// TODO: remove the #if 0 / #endif once Enemy.cpp exists
#if 0
// Landing on an enemy from above defeats it, any other contact hurts the player
void Player::resolveEnemyCollision(Enemy& e) {
    if (!e.isAlive() || !intersects(e))
        return;

    FloatRect eb = e.getBounds();
    bool fallingOntoIt = velocityY > 0.f && (y + height) - eb.position.y < 20.f;

    if (fallingOntoIt) {
        e.defeat();
        bounce();
        *this += STOMP_POINTS;
    } else {
        takeDamage();
    }
}
#endif


void Player::bounce() {
    velocityY = -jumpStrength * 0.6f;
    onGround = false;
}


// TODO: remove the #if 0 / #endif once Coin.cpp exists
#if 0
void Player::collectCoin(Coin& c) {
    if (c.isCollected() || !intersects(c))
        return;
    *this += c.collect();
}
#endif


void Player::takeDamage() {
    if (isInvincible())
        return;

    lives--;
    if (lives > 0)
        respawn();
}


void Player::respawn() {
    x = spawnX;
    y = spawnY;
    velocityX = 0.f;
    velocityY = 0.f;
    onGround = false;
    invincibleTimer = INVINCIBLE_TIME;
}


void Player::setSpawn(float x, float y) {
    spawnX = x;
    spawnY = y;
}


bool Player::isInvincible() const {
    return invincibleTimer > 0.f;
}


bool Player::isAlive() const {
    return lives > 0;
}


void Player::resetRun() {
    lives = START_LIVES;
    score = 0;
    respawn();
    invincibleTimer = 0.f;
}


int Player::getScore() const {
    return score;
}


int Player::getLives() const {
    return lives;
}


Player& Player::operator+=(int points) {
    score += points;
    return *this;
}
