#ifndef SLITERATOR_H
#define SLITERATOR_H

#include "SLNode.h"

template <typename T>
class SLIterator {
private:
    SLNode<T>* node;

public:
    SLIterator(SLNode<T>* n) {
        node = n;

    }

    T data() {
        return node->data;
    }

    void next() {
        if (node->next != nullptr)
            node = node->next;
    }
};


#endif
