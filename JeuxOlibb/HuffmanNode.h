#ifndef TP3_HUFFMANNODE_H
#define TP3_HUFFMANNODE_H

class HuffmanNode {
public:
    char data;
    unsigned int occurence;
    HuffmanNode *gauche,*droite;

    HuffmanNode(char d, unsigned int o, HuffmanNode* g, HuffmanNode* dr) {
        data = d;
        occurence = o;
        gauche = g;
        droite = dr;
    }
};


#endif