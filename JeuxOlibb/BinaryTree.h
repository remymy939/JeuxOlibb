#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "TLNode.h"
#include "Queue.h"
#include "Data.h"
#include "Liste.h"

template <typename T>
class BinaryTree {
private:
    TLNode<T>* root;
    unsigned int count;

    void preOrder(TLNode<T>* runner, Queue<Data<T>*>* traversal) {
        traversal->push(new Data<T>(runner->data));
        if (runner->gauche)
            preOrder(runner->gauche, traversal);
        if (runner->droite)
            preOrder(runner->droite,traversal);
    }

    void inDisOrder(TLNode<T>* runner, Queue<Data<T>*>* traversal) {
        if (runner->droite)
            inDisOrder(runner->droite, traversal);
        traversal->push(new Data<T>(runner->data));
        if (runner->gauche)
            inDisOrder(runner->gauche,traversal);
    }

    void postOrder(TLNode<T>* runner, Queue<Data<T>*>* traversal) {
        if (runner->gauche)
            postOrder(runner->gauche, traversal);
        if (runner->droite)
            postOrder(runner->droite,traversal);
        traversal->push(new Data<T>(runner->data));
    }

    void leftRotation(TLNode<T>* runner){
        T temp = runner->data;
        runner->data = runner->droite->data;
        runner->droite->data = temp;
        runner->gauche = runner->droite;
        runner->droite = runner->droite->droite;
    }

    void rightRotation(TLNode<T>* runner){
        T temp = runner->data;
        runner->data = runner->gauche->data;
        runner->gauche->data = temp;
        runner->droite = runner->gauche;
        runner->gauche = runner->gauche->gauche;
    }

    void balancing(TLNode<T>* runner) {
        if (getHeight(runner->gauche) - getHeight(runner->droite) == 2)
            rightRotation(runner);
        if (getHeight(runner->droite) - getHeight(runner->gauche) == -2)
            leftRotation(runner);
    }

    void addR(T data, TLNode<T>* runner) {
        if (data > runner->data)
            if (runner->droite)
                addR(data,runner->droite);
            else
                runner->droite = new TLNode<T>(data, nullptr, nullptr, runner);
        else
            if (runner->gauche)
                addR(data,runner->gauche);
            else
                runner->gauche = new TLNode<T>(data, nullptr, nullptr, runner);
        //balancing(runner); project défectueux
    }

    TLNode<T>* search(T data, TLNode<T>* runner) {
        if (data > runner->data)
            if (runner->droite)
                return search(data,runner->droite);
            else
                return nullptr;
        if (data < runner->data)
            if (runner->gauche)
                return search(data,runner->gauche);
            else
                return nullptr;
        return runner;
    }


public:
    BinaryTree() {
        root = nullptr;
        count = 0;
    }

    ~BinaryTree() {
        clear();
    }

    void clear() {
        while (root)
            remove(root->data);
    }

    void addR(T data) {
        if (!root)
            root = new TLNode<T>(data, nullptr, nullptr, nullptr);
        else {
            addR(data, root);
        }
        count++;
    }

    int getHeight(TLNode<T>* noeud) {
        int gauche = 0, droite = 0;
        if (noeud->gauche)
            gauche = getHeight(noeud->gauche) + 1;

        if (noeud->droite)
            droite = getHeight(noeud->droite) + 1;

        if (gauche >= droite)
            return gauche;
        else
            return droite;
    }

    void add(T data) {
        if (!root)
            root = new TLNode<T>(data, nullptr, nullptr, nullptr);
        else {
            TLNode<T>* runner = root;
            while (runner) {
                if (data > runner->data)
                    if (runner->droite)
                        runner = runner->droite;
                    else {
                        runner->droite = new TLNode<T>(data, nullptr, nullptr, runner);
                        runner = nullptr;
                    }
                else
                if (runner->gauche)
                    runner = runner->gauche;
                else {
                    runner->gauche = new TLNode<T>(data, nullptr, nullptr, runner);
                    runner = nullptr;
                }
            }
        }
        count++;
    }

    bool search(T data) {
        if (!root) return false;
        return search(data, root);
    }

    void remove(T toRemove) {
        TLNode<T>* pRemove = search(toRemove,root);
        if (pRemove != nullptr) {
            if (pRemove->droite and pRemove->gauche) {
                TLNode<T>* temp = pRemove->gauche;
                while (temp->droite)
                    temp = temp->droite;
                T tempValue = temp->data;
                remove(temp->data);
                pRemove->data = tempValue;
                count++;
            }
            else if (!pRemove->droite xor !pRemove->gauche) {
                if (pRemove == root) {
                    if (pRemove->gauche)
                        root = pRemove->gauche;
                    else
                        root = pRemove->droite;

                    root->parent = nullptr;
                }
                else if (pRemove == pRemove->parent->gauche) {
                    pRemove->parent->gauche = (pRemove->gauche)? pRemove->gauche: pRemove->droite;
                    if (pRemove->gauche)
                        pRemove->gauche->parent = pRemove->parent;
                    else
                        pRemove->droite->parent = pRemove->parent;
                }
                else {
                    pRemove->parent->droite = (pRemove->gauche)? pRemove->gauche: pRemove->droite;
                    if (pRemove->gauche)
                        pRemove->gauche->parent = pRemove->parent;
                    else
                        pRemove->droite->parent = pRemove->parent;
                }
                delete pRemove;
            }
            else {
                if (pRemove == root){
                    delete pRemove;
                }
                else {
                    if (pRemove == pRemove->parent->gauche)
                        pRemove->parent->gauche = nullptr;
                    else
                        pRemove->parent->droite = nullptr;
                    delete pRemove;
                }
            }
            if (--count == 0)
                root = nullptr;
        }
    }

    Queue<Data<T>*>* preOrder() { //element gauche droite
        if (root) {
            Queue<Data<T>*>* traversal = new Queue<Data<T>*>();
            preOrder(root, traversal);
            return traversal;
        }
        else
            return nullptr;
    }

    Queue<Data<T>*>* inDisOrder() { //gauche element droite tri un arbre en croisssant
        if (root) {
            Queue<Data<T>*>* traversal = new Queue<Data<T>*>();
            inDisOrder(root, traversal);
            return traversal;
        }
        else
            return nullptr;
    }

    Queue<Data<T>*>* postOrder() { //gauche droite element
        if (root) {
            Queue<Data<T>*>* traversal = new Queue<Data<T>*>();
            postOrder(root, traversal);
            return traversal;
        }
        else
            return nullptr;
    }

    Queue<Data<T>*>* breadthFirst(Queue<TLNode<T>*>* traversal, Queue<Data<T>*>* ordered ) {
        if (root)
            traversal->push(root);
        while (traversal->size() != 0){
            if (traversal->front()->gauche)
                traversal->push(traversal->front()->gauche);

            if (traversal->front()->droite)
                traversal->push(traversal->front()->droite);

            ordered->push(new Data<T>(traversal->front()->data));
            traversal->pop();
        }
        return ordered;
    }

    unsigned int size() {
        return count;
    }

};



#endif