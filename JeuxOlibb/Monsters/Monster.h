#ifndef TP4_MONSTER_H
#define TP4_MONSTER_H



#include "iostream"
#include <math.h>

const double SPEEDUNIT = 0.0625;
enum speedType {veryLowSpeed = 2, lowSpeed = 4, mediumSpeed = 5, fastSpeed = 10, veryFastSpeed = 16};

using namespace std;

class Monster {
public:
    double speed = mySpeed * SPEEDUNIT;
    string name = "monsterinc";
    int gold = 100;
    double hitPoint = 100, currentHitPoint = 100;
    int damage = 1;
    speedType mySpeed = lowSpeed;
    double x, y;
    int spawnTime = 20; // a enlever quand les enfant seront créer (la valeur de base)
    int texture;
    double parcourt = 0;
    int nbCase = 0;
    DLIterator<Position*>* pathing;

    Monster(int spawnTime, speedType mySpeed) {
        this->spawnTime = spawnTime * this->spawnTime; // a enlever quand les enfant seront créer
        this->x = pathList->get(0)->x * ICON_WIDTH;
        this->y = pathList->get(0)->y * ICON_HEIGHT;
        this->pathing = pathList->begin();
        this->pathing->next();
        this->speed = mySpeed * SPEEDUNIT;
    }

    ~Monster() {
        delete pathing;
    }

    void Deplacement() { //todo sa chie quand le ICON_WIDTH/HEIGHT n'est pas divisible par le speed du monstre

        if (pathing->get()->y * ICON_HEIGHT < y) { //nord
            y -= speed;
        }
        else if (pathing->get()->x * ICON_WIDTH > x) { //est
            x += speed;
            //cout << x << "  " << y;
        }
        else if (pathing->get()->x * ICON_WIDTH  < x) { //ouest
            x -= speed;
        }
        else if (pathing->get()->y * ICON_HEIGHT > y) { //sud
            y += speed;
        }
        parcourt += speed;
        if (trunc(parcourt/ICON_WIDTH) > nbCase) {
            nbCase++;
            this->pathing->next();
        }
        if (nbCase == pathing->get()->size) {
            this->currentHitPoint = 0;
            currentHP -= this->damage;
        }

    }

};


#include "MinouRapide.h"
#include "SnowSprites.h"
#include "FlyingCabbage.h"

#endif
