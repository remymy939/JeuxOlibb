#define ICON_WIDTH 35
#define ICON_HEIGHT 35

int currentHP = 10, maxHP = 10;
int gold = 500;

#include "Labyrinth.h"
#define GRID_WIDTH 20
#define GRID_HEIGHT 20
#include "Towers/Tower.h"
#include "Liste.h"
#include "Position.h"
Liste<Position*>* pathList = new Liste<Position*>();
#include "Monsters/Monster.h"

Liste<Monster*>* monsterToAdd = new Liste<Monster*>();
Liste<Monster*>* monsterList = new Liste<Monster*>();

#include "WaveStone.h"
#include "BinaryTree.h"
#include "Projectile.h"

Liste<Tower*>* towerList = new Liste<Tower*>();
BinaryTree<int>* position = new BinaryTree<int>();
Liste<Projectile*>* projectileList = new Liste<Projectile*>();
Liste<WaveStone*>* waveList = new Liste<WaveStone*>();
Liste<Tower*>* towerArchetype = new Liste<Tower*>();

#include "TP4.h"

int tower = -1;

using namespace std;

void createButton(void (*function)(), string Label, int xPos, int yPos, int xCursor, int yCursor, int width, int height, bool enabled, int texture = 10) {
    tp4->drawMisc(texture, xPos, yPos, height, width);
    tp4->drawString(Label, (xPos + (width/2)) - (tp4->getStringWidth(Label)/2), yPos + (height/2) - 6);
    if (enabled and ((xCursor > xPos) and (xCursor < (xPos + width)) and (yCursor > yPos) and (yCursor < (yPos + height))))
        (*function)();
}

bool isLonger(int x, int y, int pathSize) {
    DLIterator<Position*>* it = pathList->begin();
    for (int i = 0; i < pathList->size(); ++i) {
        if ((x == it->get()->x) and (y == it->get()->y) and ((pathSize + 1) >= it->get()->size)) {
            delete it;
            return true;
        }
        else
            it->next();
    }
    delete it;
    return false;
}

bool PathFinding(int x, int y, int pathSize) {
    if (!(LEVEL[x][y - 1] == MUR) and !(isLonger(x, y - 1, pathSize)))
        pathList->add(new Position(x, y - 1, pathSize + 1));
    if (!(LEVEL[x][y + 1] == MUR) and !(isLonger(x, y + 1, pathSize)))
        pathList->add(new Position(x, y + 1, pathSize + 1));
    if (!(LEVEL[x + 1][y] == MUR) and !(isLonger(x + 1, y, pathSize)))
        pathList->add(new Position(x + 1, y, pathSize + 1));
    if (!(LEVEL[x - 1][y] == MUR) and !(isLonger(x - 1, y, pathSize)))
        pathList->add(new Position(x - 1, y, pathSize + 1));
    return !((LEVEL[x][y - 1] == SORTIE) or (LEVEL[x][y + 1] == SORTIE) or (LEVEL[x + 1][y] == SORTIE) or (LEVEL[x - 1][y] == SORTIE));
}

void iFoundThePath() {
    int i = 0;
    DLIterator<Position*>* it = pathList->begin();
    while (PathFinding(it->get()->x, it->get()->y, it->get()->size)) it->next();
    delete it;
}

Position* Startingpoint() {
    for (int y = 0; y < GRID_HEIGHT; ++y)
        for (int x = 0; x < GRID_WIDTH; ++x)
            if (LEVEL[x][y] == ENTREE) return new Position(x, y, 0);
}

void createWave() {
    for (int i = 0; i < 20; ++i) {
        waveList->add(new WaveStone(i, 0, 120*(i+1)));
    }
    DLIterator<WaveStone*>* it = waveList->begin();
    for (int i = 0; i < waveList->size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            it->get()->addMonster(new MinouRapide(j));
            if ((i % 3) == 0 && j == 0) {
                it->get()->addMonster(new SnowSprites(j));
            }
            for (int k = 0; k < 5; ++k) {
                it->get()->addMonster(new FlyingCabbage(j+k));
            }
        }
        it->next();
    }
    delete it;
}

void onInit() {
    towerArchetype->add(new Tower);
    towerArchetype->add(new FlameThrower);
    Position* Start = Startingpoint();
    pathList->add(new Position(Start->x, Start->y, Start->size));
    delete Start;
}

void onKeyPress(int key)
{
    void buyTower();
    void buyFlameThrower();

    if (key == 49){
        buyTower();
    }
    if (key == 50){
        buyFlameThrower();
    }
}

void onMouseClick(int x, int y, bool button, bool ctrl) {
    int positionPixel = POS;
    if (LEVEL[x/ICON_WIDTH][y/ICON_HEIGHT] == MUR) {
        /* switch (tp4->drawMenu(x, y, MENU_ALL)) {
             case MENU_NEW_TOWER:
                 if (!position->search(positionPixel) and (gold >= 100)) {
                     towerList->add(new Tower((positionPixel%GRID_WIDTH) * ICON_WIDTH, (positionPixel/GRID_HEIGHT) * ICON_HEIGHT));
                     gold -= 100;
                     position->add(positionPixel);
                 }
                 break;
             case 2:
                 if (!position->search(positionPixel) and (gold >= 150)) {
                     towerList->add(new FlameThrower((positionPixel%GRID_WIDTH) * ICON_WIDTH, (positionPixel/GRID_HEIGHT) * ICON_HEIGHT));
                     gold -= 100;
                     position->add(positionPixel);
                 }
                 break;
         }
     for (int j = 0; j < pathList->size(); ++j) {
         cout << pathList->get(j)->x << " " << pathList->get(j)->y << endl;
     }*/
        cout << tower << endl;

        if (!position->search(positionPixel) and (gold >= 100) and (tower == 0) and (x < MENU)) {
            towerList->add(
                    new Tower((positionPixel % GRID_WIDTH) * ICON_WIDTH, (positionPixel / GRID_HEIGHT) * ICON_HEIGHT));
            gold -= 100;
            position->add(positionPixel);
            tower = -1;
        }
        if (!position->search(positionPixel) and (gold >= 250) and (tower == 1)and (x < MENU)) {
            towerList->add(new FlameThrower((positionPixel % GRID_WIDTH) * ICON_WIDTH,
                                            (positionPixel / GRID_HEIGHT) * ICON_HEIGHT));
            gold -= 250;
            position->add(positionPixel);
            tower = -1;
        }
        cout << tower;
    }
}

void DrawRange(int x, int y) {
    DLIterator<Tower*>* it = towerList->begin();
    for (int i = 0; i < towerList->size(); ++i) {
        if (((it->get()->x <= x) and ((it->get()->x + ICON_WIDTH) > x)) and
            ((it->get()->y <= y) and (it->get()->y + ICON_HEIGHT) > y)) {
            tp4->drawRange(it->get()->x - (it->get()->range / 2) + (ICON_WIDTH / 2),
                           it->get()->y - (it->get()->range / 2) + (ICON_HEIGHT / 2), it->get()->range);
        }
        it->next();
    }
    delete it;
}

void pause() {
    tp4->pause = !tp4->pause;
}

void setSpeed1() {
    tp4->speed = 1;
}

void setSpeed3() {
    tp4->speed = 3;
}

void setSpeed9() {
    tp4->speed = 9;
}

void buyTower(){
    tower = 0;

}
void buyFlameThrower(){
    tower = 1;
}

void allButton(int x, int y, bool click) {
    createButton(&pause, "pause", MENU + 95, 0, x, y, 40, 25, click);
    createButton(&setSpeed1, "X1", MENU + 20, 0, x, y, 25, 25, click);
    createButton(&setSpeed3, "X3", MENU + 45, 0, x, y, 25, 25, click);
    createButton(&setSpeed9, "X9", MENU + 70, 0, x, y, 25, 25, click);
    createButton(&buyTower, "Tower", MENU + 10, 50, x, y, 75, 75, click, 1);
    createButton(&buyFlameThrower, "FlameThrower", MENU + 85, 50, x, y, 75, 75, click, 2);
}

void onDraw(int x, int y, bool click) {
    int positionPixel = POS;
    for (int y = 0; y <= GRID_HEIGHT-1; ++y)
        for (int x = 0; x <= GRID_WIDTH-1; ++x)
            tp4->drawMisc(LEVEL[x][y], x * ICON_WIDTH, y * ICON_HEIGHT);

    DLIterator<Tower*>* itT = towerList->begin();
    for (int i = 0; i < towerList->size(); ++i) {
        tp4->drawTower(itT->get()->texture, itT->get()->x, itT->get()->y);
        itT->next();
    }
    DLIterator<Monster*>* itM = monsterList->begin();
    for (int i = 0; i < monsterList->size(); ++i) {
        tp4->drawMonster(itM->get()->texture, itM->get()->x, itM->get()->y);
        tp4->drawHP(itM->get());
        itM->next();
    }
    DLIterator<Projectile*>* itP = projectileList->begin();
    for (int i = 0; i < projectileList->size(); ++i) {
        tp4->drawProjectile(itP->get()->texture, itP->get()->x - (itP->get()->size/2) + (ICON_WIDTH/2), itP->get()->y - (itP->get()->size/2) + (ICON_HEIGHT/2), itP->get()->size);
        itP->next();
    }
    DLIterator<WaveStone*>* itW = waveList->begin();
    for (int i = 0; i < waveList->size(); ++i) {
        tp4->drawWaveStone(itW->get()->x, itW->get()->y);
        itW->next();
    }
    if (position->search(positionPixel)) DrawRange(x, y);
    tp4->drawCoordinate(x - ((MENU < x) * MENU), y, MENU + 20, 450);
    if (monsterList->size() > 0) tp4->drawMonsterHP(monsterList->get(0)->currentHitPoint, monsterList->get(0)->hitPoint, MENU + 20, 400); else tp4->drawMonsterHP(0,0,MENU+20,400);
    tp4->drawString("Gold:", MENU + 20, 480);
    tp4->drawInt(gold, MENU + 50, 480);
    tp4->drawInt(monsterList->size(), x - 5, y - 10);
    allButton(x, y, click);
    tp4->drawStatusBar(currentHP, maxHP);
    if (tower != -1) {
        tp4->drawTower(towerArchetype->get(tower)->texture, x - (ICON_WIDTH/2), y - (ICON_HEIGHT/2));
        tp4->drawRange(x - (towerArchetype->get(tower)->range/2), y - (towerArchetype->get(tower)->range/2), towerArchetype->get(tower)->range);
    }
    delete itT, itM, itP, itW;
}

void onClose() {
    delete waveList;
    delete towerList;
    delete monsterList;
    delete projectileList;
    delete pathList;
    delete monsterToAdd;
    delete position;
    delete towerArchetype;
}


int main(int argc, char *argv[]) {
    tp4 = new TP4(onInit, onMouseClick, onDraw, onKeyPress, onClose);
    srand(time(NULL));
    iFoundThePath();
    createWave();
    while (tp4->isRunning());
    delete tp4;
    return 0;
}