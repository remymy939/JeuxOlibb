//
// Created by olibb on 23/01/2017.
//

#ifndef TP4_FLAMETHROWER_H
#define TP4_FLAMETHROWER_H

#include "Tower.h"

class FlameThrower:public Tower{
public:
    FlameThrower(int x, int y) : Tower(x, y) {
        /*string name;
        int cost = 100;
        double damage = 33;
        int range = 200;
        int cooldown = 20;
        int internalCooldown = 20;
        int x, y;
        int texture = 0;
        int projectileSize = 10;
    */
        this->name = "FlameThrower";
        this->damage = 3;
        this->cooldown = 2;
        this->range = 125;
        this->internalCooldown = cooldown;
        this->texture = 1;
        this->projectileSize = 25;
        this->cost = 250;
    }

    FlameThrower() {
        name = "FlameThrower";
        cost = 250;
        damage = 15;
        range = 125;
        cooldown = 2;
        texture = 1;
        projectileSize = 25;
    }
};

#endif //TP4_FLAMETHROWER_H
