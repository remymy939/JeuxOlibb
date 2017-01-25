#ifndef TRREEEE_TLNODE_H
#define TRREEEE_TLNODE_H


template <typename T>
class TLNode {
public:
    T data;
    TLNode<T>* gauche;
    TLNode<T>* droite;
    TLNode<T>* parent;


    TLNode(T d, TLNode<T>* g, TLNode<T>* dr, TLNode<T>* p) {
        data = d;
        gauche = g;
        droite = dr;
        parent = p;
    }
};

#endif
