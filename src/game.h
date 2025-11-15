#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <windows.h>
#include "piece.h"

class Game {
    std::vector<std::vector<Piece*>> board;
    char turn;
    int mode;

public:
    HANDLE hcon;
    std::string logo;

    Game(int mode);
    bool isCheck(char color);
    void print();
    void saveGameState(const std::string& filename);
    void loadGameState(const std::string &filename);
    char getTurn();
    bool isCheckMate(char color);
    bool isStaleMate(char color);
    bool isPromAvail(int x, int y);
    bool promote(int x, int y, char opt, char color);
    bool isCheckAt(int x, int y, char color);
    bool move(int x1, int y1, int x2, int y2);
};

#endif