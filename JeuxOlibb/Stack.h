#ifndef STACK_H
#define STACK_H

#include "SLNode.h"
#include "SLIterator.h"

template <typename T>
class Stack {
private:
    SLNode<T>* SP;
    unsigned int count;

public:
    Stack() {
        SP = nullptr;
        count = 0;
    }

    ~Stack() {
        while (SP != nullptr) {
            pop();
        }
    }

    void push(T data) {
        SP = new SLNode<T>(data, SP);
        ++count;
    }

    void pop() {
        if (SP != nullptr) {
            SLNode<T>* toDelete = SP;
            SP = SP->next;
            delete toDelete;
            --count;
        }
    }

    T top() {
        if (count != 0)
            return SP->data;
    }

    unsigned int size() {
        return count;
    }

    SLIterator<T>* begin() {
        if (SP != nullptr) return new SLIterator<T>(SP);
    }



};

#endif