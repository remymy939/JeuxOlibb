#ifndef DLITERATOR_H
#define DLITERATOR_H

#include "DLNode.h"

template <typename T>
class DLIterator {
private:
    DLNode<T>* node;

public:
    DLIterator(DLNode<T>* n) {
        node = n;
    }

    DLIterator(DLIterator<T>* n) {
        node = n->node;
    }

    T get() {
        return node->data;
    }

    void set(T data) {
        node->data = data;
    }

    bool next() {
        if (node->next != nullptr)
            node = node->next;
        else
            return false;
        return true;
    }

    bool prev() {
        if (node->prev != nullptr)
            node = node->prev;
        else
            return false;
        return true;
    }
};


#endif
