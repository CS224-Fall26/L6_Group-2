#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include vector<Platforms>

class Level {
    private:
        int levelNumber;
        string filePath;
        float levelWidth;
        float levelHeight;
        float requiredCoins;
        float spawnX;
        float spawnY;
        FloatRect exitBounds;
        vector<Platform> platforms;



    public:

    
};

#endif