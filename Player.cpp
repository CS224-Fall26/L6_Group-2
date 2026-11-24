#include "Player.hpp"
#include <algorithm>
#include <optional>

using namespace sf;
using namespace std;

namespace {
    const float GRAVITY = 1400.f;
    const float MAX_FALL_SPEED = 900.f;
    const int   START_LIVES = 3;
}


Player::Player(float x, float y)
    : GameObject(x, y, 32.f, 48.f),
      name("Player"),
      lives(START_LIVES),
      score(0),
      velocityX(0.f),
      velocityY(0.f),
      moveSpeed(200.f),
      jumpStrength(620.f),
      onGround(false),
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
    velocityY += GRAVITY * deltaTime;
    velocityY = min(velocityY, MAX_FALL_SPEED);

    x += velocityX * deltaTime;
    y += velocityY * deltaTime;

    if (x < 0.f) x = 0.f;

    onGround = false;
}


void Player::draw(RenderWindow &window) {
    RectangleShape body({width, height});
    body.setPosition({x, y});
    body.setFillColor(Color(66, 133, 244));
    body.setOutlineColor(Color(25, 60, 140));
    body.setOutlineThickness(-2.f);
    window.draw(body);
}


void Player::jump() {
    if (onGround) {
        velocityY = -jumpStrength;
        onGround = false;
    }
}


void Player::resolveCollision(const Platform& p) {
    FloatRect pb = p.getBounds();
    optional<FloatRect> overlap = getBounds().findIntersection(pb);
    if (!overlap)
        return;

    if (overlap->size.y < overlap->size.x) {
        if (y < pb.position.y) {
            y -= overlap->size.y;
            onGround = true;
        } else {
            y += overlap->size.y;
        }
        velocityY = 0.f;
    } else {
        if (x < pb.position.x)
            x -= overlap->size.x;
        else
            x += overlap->size.x;
    }
}


void Player::bounce() {
    velocityY = -jumpStrength * 0.6f;
    onGround = false;
}


void Player::respawn() {
    x = spawnX;
    y = spawnY;
    velocityX = 0.f;
    velocityY = 0.f;
    onGround = false;
}


void Player::setSpawn(float x, float y) {
    spawnX = x;
    spawnY = y;
}

bool Player::isAlive() const {
    return lives > 0;
}


void Player::resetRun() {
    lives = START_LIVES;
    score = 0;
    respawn();
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
