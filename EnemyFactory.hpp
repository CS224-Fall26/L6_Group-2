#ifndef ENEMYFACTORY_HPP
#define ENEMYFACTORY_HPP
#include <memory>
#include "Enemy.hpp"
#include "Patroller.hpp"
#include "Flyer.hpp"

using namespace std;


class EnemyFactory {
    public:
        static unique_ptr<Enemy> create(char typeCode, float x, float y);
};

#endif
