class GameObject {
    protected:
        flaot x;
        float y;
        float width;
        float height;

    public:
        virtual ~GameObject() = default;
        virtual void update(float deltaTime) = 0;
        void draw(window &renderWindow);
        

}