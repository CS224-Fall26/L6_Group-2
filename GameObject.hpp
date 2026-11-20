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
        virtual ~GameObject() = default;
        virtual void update(float deltaTime) = 0;
        void draw(window &renderWindow);
        FloatRect getBounds() const;
        bool intersects(const GameObject& other) const;
        bool intersects(const FloatRect& rect) const;
};

#endif