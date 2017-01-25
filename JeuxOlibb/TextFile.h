#ifndef TP4_TEXTFILE_H
#define TP4_TEXTFILE_H

#include <iostream>


class TextFile {
public:
    std::string name;
    std::string content;

    TextFile(std::string Name, std::string Content) {
        name = Name;
        content = Content;
    }
};


#endif
