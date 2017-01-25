#include "HuffmanNode.h"
#include "sstream"
using namespace std;
string charEncodage;

void QuickSort(Liste<HuffmanNode*>* liste, DLIterator<HuffmanNode*>* iBegin, //Trie selon les occurences
               DLIterator<HuffmanNode*>* iEnd) {

    DLIterator<HuffmanNode*> *gauche = new DLIterator<HuffmanNode*>(iBegin),
            *droite = new DLIterator<HuffmanNode*>(iEnd),
            *pivot = gauche;
    HuffmanNode* temp;

    while (gauche->get() != droite->get()) {
        if (gauche->get()->occurence > droite->get()->occurence) {

            temp = gauche->get();
            gauche->set(droite->get());
            droite->set(temp);

            (pivot->get() == droite->get())? pivot = gauche :pivot = droite;
        }
        (pivot == droite)? gauche->next() : droite->prev();
    }

    if (pivot->get() != iBegin->get())
        if (pivot->prev() and pivot->get() != iBegin->get()) {
            QuickSort(liste, iBegin, pivot);
            pivot->next();
        }

    if (pivot->get() != iEnd->get())
        if (pivot->get() != iEnd->get() and pivot->next())
            QuickSort(liste, pivot, iEnd);

    delete gauche, droite;
}

void Order(HuffmanNode* runner, string numEncodage) { //Donne l'encodage de chacun des characteres
    if (runner->data == '\0') {
        if (runner->gauche)
            Order(runner->gauche, numEncodage + "0");
        if (runner->droite)
            Order(runner->droite, numEncodage + "1");
    }
    else {
        string character(1, runner->data);
        charEncodage += character + ":" + numEncodage + ",";
    }
}

void treeDelete(HuffmanNode* runner) { //Delete les elements de l'arbre sauf le premier qui sera détruit dans la liste
    if (runner->gauche)
        treeDelete(runner->gauche);
    if (runner->droite)
        treeDelete(runner->droite);
    if (!runner->gauche and !runner->droite)
        delete runner;
}

string Compress(string content) {
    Liste<HuffmanNode*>* huffManTree = new Liste<HuffmanNode*>();

    for(unsigned int i = 0; i < content.size(); ++i) { //creation de la liste
        DLIterator<HuffmanNode*>* it = huffManTree->begin();
        bool occurence = false;
        for (int j = 0; j < huffManTree->size(); ++j) {
            if (it->get()->data == content[i] and !occurence) {
                it->get()->occurence++;
                occurence = true;
            }
            else
                it->next();
        }
        delete it;
        if (!occurence)
            huffManTree->add(new HuffmanNode(content[i], 1, nullptr, nullptr));
    }

    string recensement;
    for (int i = 0; i < huffManTree->size(); ++i) { //creation du recensement
        recensement += huffManTree->get(i)->data;
        recensement += ':';
        ostringstream occurence;
        occurence << huffManTree->get(i)->occurence;
        recensement += occurence.str();
        recensement += ',';
    }

    while (huffManTree->size() != 1) { // transformation de la liste en arbre
        DLIterator<HuffmanNode*>* iBegin = huffManTree->begin();
        DLIterator<HuffmanNode*>* iEnd = huffManTree->end();

        QuickSort(huffManTree, iBegin, iEnd);
        huffManTree->add(new HuffmanNode('\0', huffManTree->get(0)->occurence + huffManTree->get(1)->occurence,
                                         huffManTree->get(0), huffManTree->get(1)));
        huffManTree->remove(0);
        huffManTree->remove(0);
        delete iBegin;
        delete iEnd;
    }

    string numEncodage;
    Order(huffManTree->get(0), numEncodage); //creation de l'encodage (stocker dans charEncodage)

    if (charEncodage.size() < 4) { // Si il n'y a qu'un type de character dans le fichier Source
        string character(1, huffManTree->get(0)->data);
        charEncodage = character + ":" + '0' + ",";
    }

    string encoder;
    int encodageIterator;
    unsigned char decimal = 0;
    char power = 8;

    for (unsigned int i = 0; i < content.size(); ++i) { //creation du fichier encoder
        encodageIterator = 1;

        bool trouve = content[i] == charEncodage[0];
        while (not trouve) {
            if (content[i] == charEncodage[encodageIterator])
                trouve = charEncodage[encodageIterator-1] == ',';
            encodageIterator++;
        }

        encodageIterator++;
        while (charEncodage[encodageIterator] != ',') {
            decimal += (charEncodage[encodageIterator++] - '0') * pow(2, --power);
            if (power == 0) {
                power = 8;
                encoder += decimal;
                decimal = 0;
            }
        }
    }
    if (power != 8)
        encoder += decimal;

    treeDelete(huffManTree->get(0));
    delete huffManTree;
    return '<' + recensement + '>' + encoder;
}

string Uncompress(string content) {
    Liste<HuffmanNode*>* huffManTree = new Liste<HuffmanNode*>();
    int size = 1;
    string resencement, occurence;
    while (content[size] != '>')
        resencement += content[size++];

    for (int i = 0; i < resencement.size(); ++i) { // refaire la liste
        char data = resencement[i];
        occurence.clear();

        i += 2;

        while (resencement[i] != ',')
            occurence += resencement[i++];
        huffManTree->add(new HuffmanNode(data, atoi(occurence.c_str()), nullptr, nullptr));
    }

    while (huffManTree->size() != 1) { // transformation de la liste en arbre
        DLIterator<HuffmanNode*>* iBegin = huffManTree->begin();
        DLIterator<HuffmanNode*>* iEnd = huffManTree->end();

        QuickSort(huffManTree, iBegin, iEnd);
        huffManTree->add(new HuffmanNode('\0', huffManTree->get(0)->occurence + huffManTree->get(1)->occurence,
                                         huffManTree->get(0), huffManTree->get(1)));
        huffManTree->remove(0);
        huffManTree->remove(0);
        delete iBegin;
        delete iEnd;
    }

    string binaryEncoder, binaryBourage, chaineBinaire;
    while (size != content.size()) { // refaire la chaine binaire
        unsigned char intData = content[++size] + 128;
        (intData < 128)? binaryBourage = '1': binaryBourage = '0';
        for (int j = 0; j < 7; ++j) {
            if (intData != 0) {
                (intData % 2 == 0)? binaryEncoder = '0' + binaryEncoder: binaryEncoder = '1' + binaryEncoder;
                intData /= 2;
            }
            else
                binaryBourage += '0';
        }
        chaineBinaire += binaryBourage + binaryEncoder;
        binaryEncoder.clear();
    }

    HuffmanNode* runner = huffManTree->get(0);
    string finalSource;
    int read = 0, iterator = 0;
    while (read != huffManTree->get(0)->occurence) { // interpreter le binaire
        while (runner->data == '\0')
            runner = (chaineBinaire[iterator++] == '0')? runner->gauche : runner->droite;
        finalSource += runner->data;
        runner = huffManTree->get(0);
        read++;
    }

    treeDelete(huffManTree->get(0));
    delete huffManTree;
    return finalSource;
}