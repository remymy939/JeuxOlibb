//
// Created by olibb on 23/01/2017.
//

#ifndef TP4_FLYINGCABBAGE_H
#define TP4_FLYINGCABBAGE_H

#include "Monster.h"

class FlyingCabbage:public Monster {
public:
    FlyingCabbage(int spawnTime):Monster(spawnTime, veryFastSpeed){
        this->gold = 05;
        this->hitPoint = this->currentHitPoint = 100;
        this->damage = 1;
        this->name = "FLYING_CABBAGE";
        this->texture = 3;
    };

};

#endif //TP4_FLYINGCABBAGE_H
