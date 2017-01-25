#ifndef DLNODE_H
#define DLNODE_H


template <typename T>
class DLNode {
public:
    T data;
    DLNode<T>* next;
    DLNode<T>* prev;


    DLNode(T d, DLNode<T>* n, DLNode<T>* p) {
        data = d;
        next = n;
        prev = p;
    }

};

#endif