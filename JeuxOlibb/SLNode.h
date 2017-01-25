#ifndef SLNODE_H
#define SLNODE_H


template <typename T>
class SLNode {
public:
    T data;
    SLNode<T>* next;

    SLNode(T d, SLNode<T>* n) {
        data = d;
        next = n;
    }

};

#endif