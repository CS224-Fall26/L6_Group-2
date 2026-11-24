#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include <SFML/Graphics.hpp>


class GameObject {
    protected:
        float x;
        float y;
        float width;
        float height;

    public:
        GameObject(float x, float y, float width, float height);
        virtual ~GameObject() = default;
        virtual void update(float deltaTime) = 0;
        virtual void draw(sf::RenderWindow &window) = 0;
        sf::FloatRect getBounds() const;
        bool intersects(const GameObject& other) const;
        bool intersects(const sf::FloatRect& rect) const;
};

#endif
