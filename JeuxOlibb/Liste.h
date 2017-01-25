#ifndef LISTE_LISTE_H
#define LISTE_LISTE_H

#include "DLIterator.h"
#include "DLNode.h"

template<typename T>
class Liste {
private:
    DLNode<T>* first;
    DLNode<T>* last;
    unsigned int count;

public:
    Liste() {
        first = nullptr;
        last = nullptr;
        count = 0;
    }

    ~Liste() {
        while (first != nullptr) {
            delete last->data;
            remove(count-1);
        }
    }

    void add(T data) {
        if (count == 0) {
            last = new DLNode<T>(data, nullptr, nullptr);
            first = last; }
        else {
            last->next = new DLNode<T>(data, nullptr,last);
            last = last->next;
        }
        count++;
    }

    bool insert(unsigned int i, T data) {
        if (i <= count) {
            if (i == count)
                add(data);
            else {
                DLNode<T>* runner;
                if (i < count/2) {
                    runner = first;
                    for (int j = 0; j != i ; ++j)
                        runner = runner->next;
                }
                else {
                    runner = last;
                    for (int j = count-i-1; j != 0 ; --j)
                        runner = runner->prev;
                }
                if (i < 0) {
                    runner->prev->next = new DLNode<T>(data, runner,runner->prev);
                    runner->prev = runner->prev->next; }
                else {
                    first->prev = new DLNode<T>(data, first, nullptr);
                    first = first->prev;
                }
                count++;
            }
        }
    }

    void remove(unsigned int i) {
        if (i < count) {
            if ((i == 0) or (i == count-1))
                if (i == 0) {
                    if (count == 1) {
                        delete first;
                        first = nullptr;
                        last = nullptr; }
                    else {
                        first = first->next;
                        delete first->prev;
                        first->prev = nullptr;
                    }
                }
                else {
                    last = last->prev;
                    delete last->next;
                    last->next = nullptr;
                }
            else {
                DLNode<T>* runner;
                if (i < count/2) {
                    runner = first;
                    for (int j = 0; j != i ; ++j)
                        runner = runner->next;
                }
                else {
                    runner = last;
                    for (int j = count-i-1; j != 0 ; --j)
                        runner = runner->prev;
                }
                runner->next->prev = runner->prev;
                runner->prev->next = runner->next;
                delete runner;

            }
            count--;
        }
    }

    T get(unsigned int i) {
        if (i < count) {
            if (i < count/2) {
                DLNode<T>* runner = first;
                for (int j = 0; j != i ; ++j)
                    runner = runner->next;
                return runner->data;
            }
            else {
                DLNode<T>* runner = last;
                for (int j = count-i-1; j != 0 ; --j)
                    runner = runner->prev;
                return runner->data;
            }
        }
    }

    void set(unsigned int i, T data) {
        DLNode<T>* runner;
        if (i < count/2) {
            runner = first;
            for (int j = 0; j != i ; ++j)
                runner = runner->next;
        }
        else {
            runner = last;
            for (int j = count-i-1; j != 0 ; --j)
                runner = runner->prev;
        }
        runner->data = data;
    }

    unsigned int size() {
        return count;
    }

    DLIterator<T>* begin() {
        if (first) return new DLIterator<T>(first);
    }

    DLIterator<T>* end() {
        if (last) return new DLIterator<T>(last);
    }
};


#endif
