#ifndef QUEUE_H
#define QUEUE_H

#include "SLNode.h"
#include "SLIterator.h"

template <typename T>
class Queue {
private:
    SLNode<T>* tail;
    unsigned int count;

public:
    Queue() {
        tail = nullptr;
        count = 0;
    }

    ~Queue() {
        while (count != 0)
            pop();
    }

    void push(T data) {
        if (count == 0) {
            tail = new SLNode<T>(data, tail);
            tail->next = tail; }
        else {
            tail->next = new SLNode<T>(data, tail->next);
            tail = tail->next; }
        count++;
    }

    void pop() {
        if (count > 0) {
            SLNode<T>* toDelete = tail->next;
            tail->next = toDelete->next;
            delete toDelete;
            if (--count == 0)
                tail = nullptr;
        }
    }

    T front() {
        if (tail->next != nullptr)
            return tail->next->data;
    }

    T back() {
        if (tail != nullptr)
            return tail->data;
    }

    unsigned int size() {
        return count;
    }

    SLIterator<T>* begin() {
        if (tail) return new SLIterator<T>(tail->next);
    }
};

#endif