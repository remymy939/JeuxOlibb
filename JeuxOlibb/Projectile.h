#ifndef TP4_PROJECTILE_H
#define TP4_PROJECTILE_H

#include "Monsters/Monster.h"

class Projectile {
public:
    int speed = 20;
    int damage = 100;
    int x, y;
    int texture;
    int size;
    Monster* target;

    Projectile (int x, int y, int size, int texture, Monster* target) {
        this->texture = 0;
        this->target = target;
        this->x = x;
        this->y = y;
        this->size = size;
    };
};


#endif
