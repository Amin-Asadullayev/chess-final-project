#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include "utils.h"

struct Position {
    int x;
    int y;
};

class Piece {
public:
    Position pos;
    bool moved;
    char color;

    Piece(char color, int x, int y);
    virtual void setPosition(int x, int y);
    virtual bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const = 0;
    virtual char getChar() const = 0;
};

class Rook : public Piece {
public:
    Rook(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};

class Bishop : public Piece {
public:
    Bishop(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};

class Knight : public Piece {
public:
    Knight(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};

class King : public Piece {
public:
    King(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};

class Pawn : public Piece {
public:
    Pawn(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};

class Queen : public Piece {
public:
    Queen(char color, int x, int y);
    bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override;
    char getChar() const override;
};
#endif