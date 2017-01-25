#include <iostream>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


#define ICON_FOLDER 0
#define ICON_TEXTFILE 2
#define ICON_COMPRESSED_FOLDER 4

#define MENU_NONE 0
#define MENU_NEW_TOWER 1
#define MENU_NEW_TEXTFILE 2
#define MENU_RENAME 4
#define MENU_DELETE 8
#define MENU_COMPRESS 16
#define MENU_UNCOMPRESS 32
#define MENU_SELECT_ALL 64
#define MENU_PERSONALISATION 128
#define MENU_ALL 255

#define NB_MONSTER 4
#define NB_TOWER 2
#define NB_Projectile 1

#define POS (x / ICON_WIDTH) + ((y / ICON_HEIGHT) * ((ICON_WIDTH * GRID_WIDTH) / ICON_WIDTH)) // position du clic
#define MENU (GRID_WIDTH*ICON_WIDTH)


class TP4 {
private:
    SDL_Window *Windows;
    SDL_Renderer *Display;
    SDL_Renderer *SDLRenderer;
    SDL_Rect SDLRect;
    int Largeur, Hauteur;
    SDL_Event Event;
    bool C, Running;
    SDL_Rect Dimension[11] = {{0, 0, ICON_WIDTH, ICON_HEIGHT}, //grandeur de ce que l'on affiche (icone)
                             {0, 0, 150, 233}, // grandeur du menu contextuelle
                             {1, 0, 147, 27},
                             {1, 1, 600, 60},
                             {0, 0, 0,   0},
                             {0, 0, 0,   0},
                             {0, 0, 600, 400}, // drawStatusBar
                             {0, 0, 0,   0},
                             {0, -10, 0, ICON_HEIGHT/5}, // barre de vie
                             {0, 0, 0, 0}, // Range
                             {0, 0, 0, 0}}; // WaveStone
    SDL_Texture *Texture[7];
    SDL_Texture* TextureMonster[NB_MONSTER];
    SDL_Texture* TextureTower[NB_TOWER];
    SDL_Texture* TextureMisc[11];
    SDL_Texture* TextureProjectile[NB_Projectile];
    TTF_Font *Font;
    int Indicateur;
    int frame3 = 0;
    Uint8 StringR, StringG, StringB, WriteR, WriteG, WriteB;
    bool Classic = false;
    const char *menuItemStrings[8] = {"Tour OP", "Nouveau fichier...", "Renommer...", "Supprimer",
                                      "Compresser", "Decompresser", "Tout selectionner", "Personalisation..."};

    void (*onInit)();

    void (*onMouseClick)(int x, int y, bool button, bool ctrl);

    void (*onDraw)(int x, int y, bool click);

    void(*onKeyPress)(int key);

    void (*onClose)();

    int RGBtoINT(unsigned char R, unsigned char G, unsigned char B) {
        int hexaDecimal = 0, power = 6;
        hexaDecimal += R / 16 * pow(16, --power);
        hexaDecimal += R % 16 * pow(16, --power);
        hexaDecimal += G / 16 * pow(16, --power);
        hexaDecimal += G % 16 * pow(16, --power);
        hexaDecimal += B / 16 * pow(16, --power);
        hexaDecimal += B % 16 * pow(16, --power);
        return hexaDecimal;
    }

public:
    bool pause = false;
    char speed = 1;
    TP4(void(*OnInit)(), void(*OnMouseClick)(int x, int y, bool button, bool ctrl), void(*OnDraw)(int x, int y, bool click), void(*OnKeyPress)(int key), void(*OnClose)()) {
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        Largeur = GRID_WIDTH*ICON_WIDTH + 165;
        Hauteur = GRID_HEIGHT*ICON_HEIGHT;
        Windows = SDL_CreateWindow("Best tower defense ever", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Largeur, Hauteur, SDL_WINDOW_RESIZABLE);
        Display = SDL_CreateRenderer(Windows, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        SDL_SetRenderDrawColor(Display, 44, 44, 44, 255); // couleur fond d'écran
        SDL_SetWindowMinimumSize(Windows, Largeur, Hauteur);
        SDLRect.w = SDLRect.h = 10;
        Dimension[7] = {Largeur - 55, Hauteur - 30, 50, 30};
        SDL_Surface *Surface = SDL_CreateRGBSurface(0, 60, 25, 32, 0, 0, 0, 0);
        SDL_FillRect(Surface, NULL, 0xEEEEEE);
        Texture[0] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);
        string fileName = "Monster/ .png";
        for (int i = 0; i < NB_MONSTER; ++i) {
            fileName[8] = i + 65;
            TextureMonster[i] = IMG_LoadTexture(Display, fileName.c_str());
        }
        fileName = "Tower/ .png";
        for (unsigned char i = 0; i < 2; ++i) {
            fileName[6] = i + 65;
            TextureTower[i] = IMG_LoadTexture(Display, fileName.c_str());
        }
        fileName = "Misc/ .png";
        for (unsigned char i = 0; i < 11; ++i) {
            fileName[5] = i + 65;
            TextureMisc[i] = IMG_LoadTexture(Display, fileName.c_str());
        }
        fileName = "Projectile/ .png";
        for (unsigned char i = 0; i < 11; ++i) {
            fileName[11] = i + 65;
            TextureProjectile[i] = IMG_LoadTexture(Display, fileName.c_str());
        }

        Surface = SDL_CreateRGBSurface(0, 600, 60, 32, 0, 0, 0, 0);
        SDL_Rect SDLRects[4] = {{0, 0,  600,      60},
                                {1, 1,  598,      58},
                                {8, 30, 600 - 16, 60 - 38},
                                {9, 31, 600 - 18, 20}};
        unsigned int SDLColors[4] = {0x737373, 0x939393, 0x808080, 0xFFFFFF};
        StringR = StringG = StringB = 255;
        WriteR = WriteG = WriteB = 0;
        for (int i = 0; i < 4; i++)SDL_FillRect(Surface, &SDLRects[i], SDLColors[i]);
        Texture[6] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);
        Texture[3] = Texture[5] = nullptr;
        Surface = SDL_CreateRGBSurface(0, 600, 400, 32, 0, 0, 0, 0);
        SDLRects[0].h = 400;
        SDLRects[1].h = 398;
        SDLRects[2].h = 400 - 38;
        SDLRects[3].h = 360;
        for (int i = 0; i < 4; i++)SDL_FillRect(Surface, &SDLRects[i], SDLColors[i]);
        Texture[4] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);
        onInit = OnInit;
        onMouseClick = OnMouseClick;
        onDraw = OnDraw;
        onKeyPress = OnKeyPress;
        onClose = OnClose;
        Font = TTF_OpenFont("arial.ttf", 12); // écriture de tout
        Surface = SDL_CreateRGBSurface(0, Dimension[1].w, Dimension[1].h, 32, 0xFF0000, 0x00FF00, 0x0000FF, 0x000000);
        SDL_FillRect(Surface, NULL, 0x888888); //Box couleur
        SDL_Surface *pStringSDLSurface;
        SDL_Rect ItemRect = {1, 1, 148, 28}; // x,y position dans la box des fonds
        SDL_Rect StringRect = {10, ItemRect.y + 9, 148, 28}; // marge du texte
        for (int i = 0; i < 8; i++) {
            SDL_FillRect(Surface, &ItemRect, 0xBBBBBB); // couleur de fond
            StringRect.y = ItemRect.y + 9; //centrage vertical du texte des bouton non-Hover
            pStringSDLSurface = TTF_RenderText_Blended(Font, menuItemStrings[i],
                                                       {0, 0, 0, 255}); // couleur du texte non Hover
            SDL_BlitSurface(pStringSDLSurface, NULL, Surface, &StringRect);
            SDL_FreeSurface(pStringSDLSurface);
            ItemRect.y += 29; // distance entre les boutons non-Hover
        }
        Texture[1] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);
        Surface = SDL_CreateRGBSurface(0, 147, 27, 32, 0xFF0000, 0x00FF00, 0x0000FF, 0x000000);
        SDL_FillRect(Surface, NULL, 0xDDDDDD); //couleur de fond onHover
        Texture[2] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);
        C = false;
        Running = true;
        onInit();
    }

    ~TP4() {
        for (int i = 0; i < 7; ++i)SDL_DestroyTexture(Texture[i]);
        for (int i = 0; i < NB_MONSTER; ++i)SDL_DestroyTexture(TextureMonster[i]);
        for (int i = 0; i < NB_TOWER; ++i)SDL_DestroyTexture(TextureTower[i]);
        for (int i = 0; i < 11; ++i)SDL_DestroyTexture(TextureMisc[i]);
        for (int i = 0; i < NB_Projectile; ++i)SDL_DestroyTexture(TextureProjectile[i]);
        TTF_CloseFont(Font);
        SDL_DestroyRenderer(Display);
        SDL_DestroyWindow(Windows);
        TTF_Quit();
        SDL_Quit();
        onClose();
    }

    int getWidth() { return Largeur; }

    int getHeight() { return Hauteur; }

    void setTitle(std::string title) { SDL_SetWindowTitle(Windows, title.c_str()); }

    void drawHP(Monster* me) {
        Dimension[8].x = me->x;
        Dimension[8].y = me->y - 6;
        Dimension[8].w = (((me->currentHitPoint) / (me->hitPoint)) * ICON_WIDTH);
        /*SDL_Surface* Surface = SDL_CreateRGBSurface(0, 60, 25, 32, 0, 0, 0, 0);
        SDL_FillRect(Surface, NULL, RGBtoINT(255 - ((me->currentHitPoint / (me->hitPoint))*255), (me->currentHitPoint / (me->hitPoint))*255, 0));
        TextureMisc[0] = SDL_CreateTextureFromSurface(Display, Surface);
        SDL_FreeSurface(Surface);*/ // lag is over 9000 so fuck this shit
        SDL_RenderCopy(Display, TextureMisc[0], NULL, &Dimension[8]);
    }

    void drawRange(int x, int y, int radius) {
        Dimension[9].x = x;
        Dimension[9].y = y;
        Dimension[9].h = radius;
        Dimension[9].w = radius;
        SDL_RenderCopy(Display, TextureMisc[7], NULL, &Dimension[9]);
    }

    void drawGameOver(int icon) {
        Dimension[9].x = Largeur/4;
        Dimension[9].y = Hauteur/4;
        Dimension[9].h = Hauteur/2;
        Dimension[9].w = Largeur/2;
        SDL_RenderCopy(Display, TextureMisc[icon], NULL, &Dimension[9]);
    }

    void drawTower(int towerType, int x, int y, int height = ICON_HEIGHT, int width = ICON_WIDTH) {
        Dimension[0].x = x;
        Dimension[0].y = y;
        Dimension[0].h = height;
        Dimension[0].w = width;
        SDL_RenderCopy(Display, TextureTower[towerType], NULL, &Dimension[0]);
    }
    
    void drawProjectile(int projectileType, int x, int y, int size) {
        Dimension[9].x = x;
        Dimension[9].y = y;
        Dimension[9].h = size;
        Dimension[9].w = size;
        SDL_RenderCopy(Display, TextureProjectile[projectileType], NULL, &Dimension[9]);
    }

    void drawMonster(int monsterType, int x, int y) {
        Dimension[0].x = x;
        Dimension[0].y = y;
        SDL_RenderCopy(Display, TextureMonster[monsterType], NULL, &Dimension[0]);
    }

    void drawMisc(int icon, int x, int y, int height = ICON_HEIGHT, int width = ICON_WIDTH) {
        Dimension[0].x = x;
        Dimension[0].y = y;
        Dimension[0].h = height;
        Dimension[0].w = width;
        SDL_RenderCopy(Display, TextureMisc[icon], NULL, &Dimension[0]);
    }

    void drawWaveStone(int x, int y) {
        Dimension[10].x = x;
        Dimension[10].y = y;
        Dimension[10].h = 120;
        Dimension[10].w = 25;
        SDL_RenderCopy(Display, TextureMisc[10], NULL, &Dimension[10]);
    }

    void drawString(std::string text, int _iX, int _iY) {
        int iW, iH;
        TTF_SizeText(Font, text.c_str(), &iW, &iH);
        SDL_Rect PositionRect = {_iX, _iY, iW, iH};
        SDL_Color StringColor = {StringR, StringG, StringB, 255};
        SDL_Surface *pTemS = TTF_RenderText_Blended(Font, text.c_str(), StringColor);
        SDL_Texture *pStringTexture = SDL_CreateTextureFromSurface(Display, pTemS);
        SDL_RenderCopy(Display, pStringTexture, NULL, &PositionRect);
        SDL_FreeSurface(pTemS);
        SDL_DestroyTexture(pStringTexture);
    }

    void drawInt(int integer, int x, int y) {
        char strElements[5];
        SDL_itoa(integer, strElements, 10);
        drawString(strElements, x, y);
    }

    void drawCoordinate(int str1, int str2, int x, int y) {
        char strX[5], strY[5];
        SDL_itoa(str1, strX, 10);
        SDL_itoa(str2, strY, 10);
        drawString("x:", x, y);
        drawString(strX, x + 10, y);
        drawString("y:", x + 40, y);
        drawString(strY, x + 50, y);
    }

    void drawMonsterHP(int str1, int str2, int x, int y) {
        char strX[5], strY[5];
        SDL_itoa(str1, strX, 10);
        SDL_itoa(str2, strY, 10);
        drawString(strX, x, y);
        drawString("/", x + 30, y);
        drawString(strY, x + 40, y);
    }

    void drawStatusBar(int selectionCount,
                       int elementCount) {
        //SDL_RenderCopy(Display, Texture[0], NULL, &Dimension[7]); // la boite est laide
        char strElements[5], strSelections[5];
        SDL_itoa(elementCount, strElements, 10);
        SDL_itoa(selectionCount, strSelections, 10);
        drawString(strSelections, Dimension[7].x + 7, Hauteur - 20);
        drawString("/", Dimension[7].x + 25, Hauteur - 20);
        drawString(strElements, Dimension[7].x + 32, Hauteur - 20);
    }

    int getStringWidth(std::string string) {
        int w;
        TTF_SizeText(Font, string.c_str(), &w, nullptr);
        return w;
    }


    std::string drawTextBox(std::string title, std::string text) {
        SDL_GetWindowSize(Windows, &Largeur, &Hauteur);
        Dimension[3].x = Largeur / 2 - Dimension[3].w / 2;
        Dimension[3].y = Hauteur / 2 - Dimension[3].h / 2;
        Dimension[4].x = Dimension[3].x + 10;
        Dimension[4].y = Dimension[3].y + 9;
        Dimension[5].x = Dimension[3].x + 15;
        Dimension[5].y = Dimension[3].y + 35;
        TTF_SizeText(Font, title.c_str(), &Dimension[4].w, &Dimension[4].h);
        SDL_Surface *STitle = TTF_RenderText_Blended(Font, title.c_str(), {StringR, StringG, StringB, 255});
        SDL_DestroyTexture(Texture[3]);
        Texture[3] = SDL_CreateTextureFromSurface(Display, STitle);
        SDL_FreeSurface(STitle);
        TTF_SizeText(Font, text.c_str(), &Dimension[5].w, &Dimension[5].h);
        STitle = TTF_RenderText_Blended(Font, text.c_str(), {0, 0, 0, 255});
        SDL_DestroyTexture(Texture[5]);
        Texture[5] = SDL_CreateTextureFromSurface(Display, STitle);
        SDL_FreeSurface(STitle);
        while (Running) {
            while (SDL_PollEvent(&Event)) {
                switch (Event.type) {
                    case SDL_KEYUP:
                        if ((Event.key.keysym.sym == 13) || (Event.key.keysym.sym == 27))Running = false;
                        else if (Event.key.keysym.sym == 8) { if (text.size() > 0)text.pop_back(); }
                        else if ((Event.key.keysym.sym == 32) ||
                                 ((Event.key.keysym.sym > 47) && (Event.key.keysym.sym < 58))) {
                            if (Dimension[5].w < 565)text.push_back(Event.key.keysym.sym);
                        } else if ((Event.key.keysym.sym > 96) && (Event.key.keysym.sym < 123)) {
                            if (Event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))Event.key.keysym.sym -= 32;
                            if (Dimension[5].w < 565)text.push_back(Event.key.keysym.sym);
                        }
                        TTF_SizeText(Font, text.c_str(), &Dimension[5].w, &Dimension[5].h);
                        STitle = TTF_RenderText_Blended(Font, text.c_str(), {WriteR, WriteG, WriteB, 255});
                        SDL_DestroyTexture(Texture[5]);
                        Texture[5] = SDL_CreateTextureFromSurface(Display, STitle);
                        SDL_FreeSurface(STitle);
                        break;
                    case SDL_WINDOWEVENT:
                        if (Event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(Windows, &Largeur, &Hauteur);
                            Dimension[3].x = Largeur / 2 - Dimension[3].w / 2;
                            Dimension[3].y = Hauteur / 2 - Dimension[3].h / 2;
                            Dimension[4].x = Dimension[3].x + 10;
                            Dimension[4].y = Dimension[3].y + 9;
                            Dimension[5].x = Dimension[3].x + 15;
                            Dimension[5].y = Dimension[3].y + 35;
                            Dimension[7].x = Largeur - 55;
                            Dimension[7].y = Hauteur - 30;
                        }
                        break;
                }
                SDL_RenderClear(Display);
                onDraw(Event.button.x, Event.button.y, false);
                SDL_RenderCopy(Display, Texture[6], NULL, &Dimension[3]);
                SDL_RenderCopy(Display, Texture[3], NULL, &Dimension[4]);
                SDL_RenderCopy(Display, Texture[5], NULL, &Dimension[5]);
                SDL_RenderPresent(Display);
            }
        }
        Running = true;
        return (Event.key.keysym.sym == 27) ? "" : text;
    }

    std::string drawTextField(std::string title, std::string text) {
        SDL_GetWindowSize(Windows, &Largeur, &Hauteur);
        Dimension[6].x = Largeur / 2 - Dimension[6].w / 2;
        Dimension[6].y = Hauteur / 2 - Dimension[6].h / 2;
        Dimension[4].x = Dimension[6].x + 10;
        Dimension[4].y = Dimension[6].y + 9;
        TTF_SizeText(Font, title.c_str(), &Dimension[4].w, &Dimension[4].h);
        SDL_Surface *STitle = TTF_RenderText_Blended(Font, title.c_str(), {StringR, StringG, StringB, 255});
        SDL_DestroyTexture(Texture[3]);
        Texture[3] = SDL_CreateTextureFromSurface(Display, STitle);
        SDL_FreeSurface(STitle);
        unsigned int actualLine = 0;
        std::string Lines[25] = {"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
                                 "", "", ""};
        while ((text.size() > 0) && (actualLine < 24)) {
            while (text.front() == '@') {
                text.erase(0, 1);
                actualLine++;
            };
            if (getStringWidth(Lines[actualLine]) < 570) {
                Lines[actualLine].push_back(text.front());
                text.erase(0, 1);
            } else++actualLine;
        }
        if (actualLine > 24)--actualLine;
        SDL_Texture *lineX[25] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                                  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
        SDL_Surface *tmpSurface;
        for (int i = 0; i <= actualLine; ++i) {
            tmpSurface = TTF_RenderText_Blended(Font, Lines[i].c_str(), {WriteR, WriteG, WriteB, 255});
            lineX[i] = SDL_CreateTextureFromSurface(Display, tmpSurface);
            SDL_FreeSurface(tmpSurface);
        }
        SDL_Rect rctText = {Dimension[6].x + 12, 0, 0, 0};
        while (Running) {
            while (SDL_PollEvent(&Event)) {
                switch (Event.type) {
                    case SDL_KEYDOWN:
                        if ((Event.key.keysym.sym == 13) || (Event.key.keysym.sym == 27))Running = false;
                        else if (Event.key.keysym.sym == 8) {
                            if (Lines[actualLine].size() > 0)
                                Lines[actualLine].pop_back();
                            else if (actualLine > 0)actualLine--;
                        } else if ((Event.key.keysym.sym == 32) ||
                                   ((Event.key.keysym.sym > 47) && (Event.key.keysym.sym < 58))) {
                            if (getStringWidth(Lines[actualLine]) > 565)++actualLine;
                            if (actualLine < 25)Lines[actualLine].push_back(Event.key.keysym.sym);
                        } else if ((Event.key.keysym.sym > 96) && (Event.key.keysym.sym < 123)) {
                            if (Event.key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))Event.key.keysym.sym -= 32;
                            if (getStringWidth(Lines[actualLine]) > 565)++actualLine;
                            if (actualLine < 25)Lines[actualLine].push_back(Event.key.keysym.sym);
                        }
                        tmpSurface = TTF_RenderText_Blended(Font, Lines[actualLine].c_str(),
                                                            {WriteR, WriteG, WriteB, 255}); // Couleur d'écriture
                        SDL_DestroyTexture(lineX[actualLine]);
                        lineX[actualLine] = SDL_CreateTextureFromSurface(Display, tmpSurface);
                        SDL_FreeSurface(tmpSurface);
                        break;
                    case SDL_WINDOWEVENT:
                        if (Event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(Windows, &Largeur, &Hauteur);
                            Dimension[6].x = Largeur / 2 - Dimension[6].w / 2;
                            Dimension[6].y = Hauteur / 2 - Dimension[6].h / 2;
                            Dimension[4].x = Dimension[6].x + 10;
                            Dimension[4].y = Dimension[6].y + 9;
                            Dimension[7].x = Largeur - 55;
                            Dimension[7].y = Hauteur - 30;
                            rctText.x = Dimension[6].x + 12;
                        }
                        break;
                }
                SDL_RenderClear(Display);
                onDraw(Event.button.x, Event.button.y, false);
                SDL_RenderCopy(Display, Texture[4], NULL, &Dimension[6]);
                SDL_RenderCopy(Display, Texture[3], NULL, &Dimension[4]);
                rctText.y = Dimension[6].y + 32;
                for (int i = 0; i <= actualLine; ++i) {
                    TTF_SizeText(Font, Lines[i].c_str(), &rctText.w, &rctText.h);
                    SDL_RenderCopy(Display, lineX[i], NULL, &rctText);
                    rctText.y += rctText.h;
                }
                SDL_RenderPresent(Display);
            }
        }
        for (int i = 0; i < 25; ++i)SDL_DestroyTexture(lineX[i]);
        Running = true;
        std::string returnString = "";
        if (Event.key.keysym.sym != 27)for (int i = 0; i <= actualLine; ++i)returnString += Lines[i];
        return returnString;
    }

    void moveProjectile(Projectile* pro) {
        double ratioX, ratioY;
        double diffX, diffY;
        diffX = pro->x - pro->target->x;
        diffY = pro->y - pro->target->y;
        ratioX = -diffX / (abs(diffX) + abs(diffY));
        ratioY = -diffY / (abs(diffX) + abs(diffY));

        if (pow(pow(diffX, 2) + pow(diffY, 2), 0.5) <= pro->speed) {
            pro->x = pro->target->x;
            pro->y = pro->target->y;
        }
        else {
            pro->x += pro->speed * ratioX;
            pro->y += pro->speed * ratioY;
        }
    }

    void monsterDeplacement() {
        DLIterator<Monster*>* it = monsterList->begin();
        for (int i = 0; i < monsterList->size(); ++i) {
            it->get()->Deplacement();
            it->next();
        }
        delete it;
    }

    void waveDeplacement() {
        DLIterator<WaveStone*>* it = waveList->end();
        for (int i = waveList->size() - 1; i >= 0; --i) {
            it->get()->y -= 1;
            if (it->get()->y < 1) {
                delete it->get();
                it->prev();
                waveList->remove(i);
            }
            else it->prev();
        }
        delete it;
    }

    void projectileDeplacement() {
        DLIterator<Projectile*>* itP = projectileList->end();
        for (int i = projectileList->size() - 1; i >= 0; --i) {
            if ((pow(itP->get()->x - itP->get()->target->x, 2) + pow(itP->get()->y - itP->get()->target->y, 2)) <= pow(itP->get()->target->speed + 1, 2)) {
                itP->get()->target->currentHitPoint -= itP->get()->damage;
                delete itP->get();
                itP->prev();
                projectileList->remove(i);
            }
            else {
                moveProjectile(itP->get());
                itP->prev();
            }
        }
        delete itP;
    }

    void towerCooldown() { //todo embriquer dans un autre
        DLIterator<Tower*>* itT = towerList->begin();
        for (int i = 0; i < towerList->size(); ++i) {
            if (itT->get()->cooldown != 0)
                itT->get()->cooldown--;
            itT->next();
        }
        delete itT;
    }

    void AmIDead(DLIterator<Monster*>* me, int i) {
        if (me->get()->currentHitPoint < 1) {
            DLIterator<Projectile*>* itP = projectileList->end();
            for (int k = projectileList->size() - 1; k >= 0; --k) {
                if (itP->get()->target == me->get()) {
                    delete itP->get();
                    itP->prev();
                    projectileList->remove(k);
                }
                else itP->prev();
            }
            if (me->get()->nbCase != me->get()->pathing->get()->size) gold += me->get()->gold;
            delete me->get();
            me->prev();
            monsterList->remove(i);
        }
        else me->prev();
    }

    attackEnnemy() { //todo revoir tout ce code sa semble etre a chier (3 iterateur embriquer) monster * tower * monster
        DLIterator<Monster*>* itMonster = monsterList->end();
        for (int i = monsterList->size() - 1; i >= 0; --i) {
            DLIterator<Tower*>* itTower = towerList->begin();
            for (int j = 0; j < towerList->size(); ++j) {
                if (pow((itTower->get()->y - ICON_HEIGHT/2) - (itMonster->get()->y - ICON_HEIGHT/2), 2) + pow((itTower->get()->x - ICON_WIDTH/2) - (itMonster->get()->x - ICON_WIDTH/2), 2) < pow(itTower->get()->range/2, 2)) {
                    if (itTower->get()->cooldown == 0) {
                        DLIterator<Monster*>* itM = monsterList->begin();
                        Monster* tempMonster = new Monster(0, fastSpeed);
                        Monster* Further = tempMonster;
                        for (int k = 0; k < monsterList->size(); ++k) {
                            if ((pow((itTower->get()->y - ICON_HEIGHT/2) - (itM->get()->y - ICON_HEIGHT/2), 2) + pow((itTower->get()->x - ICON_WIDTH/2) - (itM->get()->x - ICON_WIDTH/2), 2) < pow(itTower->get()->range/2, 2))) {
                                if (itM->get()->parcourt > Further->parcourt) Further = itM->get();
                            }
                            itM->next();
                        }
                        projectileList->add(new Projectile(itTower->get()->x, itTower->get()->y, itTower->get()->projectileSize, 0, Further));
                        itTower->get()->cooldown = itTower->get()->internalCooldown;
                        delete tempMonster;
                        delete itM;
                    }
                }
                itTower->next();
            }
            AmIDead(itMonster, i);
            delete itTower;
        }
        delete itMonster;
    }

    void monsterToBattleField() {
        DLIterator<Monster*>* it = monsterToAdd->end();
        for (int i = monsterToAdd->size() - 1; i >= 0; --i) {
            if (--it->get()->spawnTime <= 0) {
                monsterList->add(it->get());
                monsterToAdd->remove(i);
            }
            it->prev();
        }
        delete it;
    }

    void Everything() {
        projectileDeplacement();
        monsterDeplacement();
        waveDeplacement();
        towerCooldown();
        attackEnnemy();
        monsterToBattleField();
    }

    bool isRunning() {
        bool click = false;
        while (SDL_PollEvent(&Event)) {
            switch (Event.type) {
                case SDL_MOUSEBUTTONDOWN:
                    onMouseClick(Event.button.x, Event.button.y, (Event.button.button == SDL_BUTTON_RIGHT), C);
                    click = true;
                    break;
                case SDL_KEYDOWN:
                    if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL)C = true;
                    break;
                case SDL_KEYUP:
                    onKeyPress(Event.key.keysym.sym);
                    if (Event.key.keysym.scancode == SDL_SCANCODE_LCTRL)C = false;
                    break;
                case SDL_WINDOWEVENT:
                    if (Event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        SDL_GetWindowSize(Windows, &Largeur, &Hauteur);
                        Dimension[7].x = Largeur - 55;
                        Dimension[7].y = Hauteur - 30;
                    }
                    break;
                case SDL_QUIT:
                    Running = false;
                    break;
            }
        }
        SDL_RenderClear(Display);
        if (currentHP < 1) {
            drawGameOver(8);
        } else if ((waveList->size() == 0) and (monsterList->size() == 0) and (monsterToAdd->size() == 0)) {
            drawGameOver(9);
        } else {
            if (!pause)
                for (int i = 0; i < speed; ++i) {
                    Everything();
                }
            onDraw(Event.button.x, Event.button.y, click);
        }
        SDL_RenderPresent(Display);
        //SDL_RenderPresent(SDLRenderer); //sa fait bugger le debuggeur pi je sais pas a quoi sa sert vraiment
        //SDL_RenderClear(SDLRenderer); //enfaite je pense que c'est totalement inutile
        return Running;
    }
};

TP4 *tp4;
