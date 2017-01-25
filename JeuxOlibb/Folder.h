#ifndef TP4_FOLDER_H
#define TP4_FOLDER_H

#include <iostream>
#include "Liste.h"
#include "TextFile.h"

class Folder {
public:
    std::string name;
    Liste<Folder*>* folders;
    Liste<TextFile*>* textFile;

    Folder(std::string Name) {
        folders = new Liste<Folder*>;
        textFile = new Liste<TextFile*>;
        name = Name;
    }

    ~Folder() {
        delete folders;
        delete textFile;
    }
};


#endif
