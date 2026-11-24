#include "Platform.hpp"

using namespace sf;


Platform::Platform(float x, float y, float w, float h)
    : GameObject(x, y, w, h) {}


// Platforms are static, so nothing to update (moving platforms could override this later)
void Platform::update(float deltaTime) {
    (void)deltaTime;
}


void Platform::draw(RenderWindow &window) {
    // Dirt body
    RectangleShape body({width, height});
    body.setPosition({x, y});
    body.setFillColor(Color(139, 94, 60));
    body.setOutlineColor(Color(90, 60, 35));
    body.setOutlineThickness(-2.f);
    window.draw(body);

    // Grass strip on top
    float grassHeight = height < 10.f ? height : 10.f;
    RectangleShape grass({width, grassHeight});
    grass.setPosition({x, y});
    grass.setFillColor(Color(76, 175, 80));
    window.draw(grass);
}
