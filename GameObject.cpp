#include "GameObject.hpp"

using namespace std;
using namespace sf;

GameObject::GameObject(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {}

FloatRect GameObject::getBounds() const {
    return FloatRect({x, y}, {width, height});
}


bool GameObject::intersects(const GameObject& other) const {
    return intersects(other.getBounds());
}


// SFML 3: FloatRect uses position/size instead of left/top/width/height
bool GameObject::intersects(const FloatRect& rect) const {
    return x < rect.position.x + rect.size.x &&
           x + width > rect.position.x &&
           y < rect.position.y + rect.size.y &&
           y + height > rect.position.y;
}
