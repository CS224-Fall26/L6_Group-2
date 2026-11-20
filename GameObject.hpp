#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP


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
};

#endif