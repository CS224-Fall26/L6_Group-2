#ifndef ASSETMANAGER_HPP
#define ASSETMANAGER_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>

using namespace sf;
using namespace std;


class AssetManager {
    private:
        map<string, SoundBuffer> soundBuffers;
        vector<Sound> activeSounds;
        map<string, Font> fonts;
        unique_ptr<Music> music;
        AssetManager();

    public:
        static AssetManager& getInstance();
        Font& getFont(const string& name);
        void playSound(const string& name);
        void playMusic(const string& name, bool loop);
        void stopMusic();
};

#endif
