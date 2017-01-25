#ifndef TP4_WAVESTONE_H
#define TP4_WAVESTONE_H


class WaveStone {
public:
    std::string name;
    Liste<Monster*>* wave;
    int x, y;

    WaveStone(int name, int x, int y) {
        this->name = std::to_string(name);
        this->x = x;
        this->y = y;
        wave = new Liste<Monster*>();
    }

    ~WaveStone() {
        DLIterator<Monster*>* it = wave->end();
        for (int i = wave->size() - 1; i >= 0; --i) {
            monsterToAdd->add(it->get());
            wave->remove(i);
            it->prev();
        }
        delete it;
        delete wave;
    }

    void addMonster(Monster* me) {
        wave->add(me);
    }
};


#endif
