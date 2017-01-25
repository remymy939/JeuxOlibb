//
// Created by olibb on 23/01/2017.
//

#ifndef TP4_SNOWSPRITES_H
#define TP4_SNOWSPRITES_H

#include "Monster.h"

class SnowSprites:public Monster{
public:
    SnowSprites(int spawnTime):Monster(spawnTime, veryLowSpeed){
        this->gold = 150;
        this->hitPoint = this->currentHitPoint = 10000;
        this->damage = 10;
        this->name = "SNOW_SPRITES";
        this->texture = 2;
    }
};

#endif //TP4_SNOWSPRITES_H
