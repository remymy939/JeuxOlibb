#ifndef TP4_TOWER_H
#define TP4_TOWER_H

#include "iostream"

using namespace std;

class Tower {
public:
    string name;
    int cost = 100;
    double damage = 33;
    int range = 200;
    int cooldown = 20;
    int internalCooldown = 20;
    int x, y;
    int texture = 0;
    int projectileSize = 10;

    Tower(int x, int y) {
        this->x = x;
        this->y = y;
    }

    Tower() {
        name = "Tour de base";
        cost = 100;
        damage = 33;
        range = 200;
        cooldown = 20;
        texture = 0;
        projectileSize = 10;
    }

};

#include "FlameThrower.h"

#endif