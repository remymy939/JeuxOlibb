//
// Created by olibb on 23/01/2017.
//

#ifndef TP4_MINOURAPIDE_H
#define TP4_MINOURAPIDE_H

#include "Monster.h"

class MinouRapide:public Monster {
public:
    MinouRapide(int spawnTime):Monster(spawnTime, veryFastSpeed){
        this->gold = 25;
        this->hitPoint = this->currentHitPoint = 1000;
        this->damage = 2;
        this->name = "MINOU_RAPIDE";
        this->texture = 1;
    };

};


#endif //TP4_MINOURAPIDE_H
