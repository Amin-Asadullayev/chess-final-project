#include "piece.h"
#include <algorithm>
#include <cmath>

bool Rook::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (pos.x == x) {
        for (int i = minstd(pos.y, y)+1; i < maxstd(pos.y, y); i++)
            if (board[x][i] != nullptr) return false;
    } else if (pos.y == y) {
        for (int i = minstd(pos.x, x)+1; i < maxstd(pos.x, x); i++)
            if (board[i][y] != nullptr) return false;
    } else return false;
    if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
    return true;
}

char Rook::getChar() const { return color == 'w' ? 'R' : 'r'; }

bool Bishop::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (absstd(x - pos.x) == absstd(y - pos.y)) {
        int dx = (x > pos.x) ? 1 : -1, dy = (y > pos.y) ? 1 : -1;
        for (int i = 1; i < absstd(x - pos.x); i++)
            if (board[pos.x + dx*i][pos.y + dy*i] != nullptr) return false;
    } else return false;
    if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
    return true;
}

char Bishop::getChar() const { return color == 'w' ? 'B' : 'b'; }

bool Knight::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (!(absstd(x - pos.x) * absstd(y - pos.y) == 2)) return false;
    if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
    return true;
}

char Knight::getChar() const { return color == 'w' ? 'N' : 'n'; }

bool King::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (pos.x == 0 && pos.y == 4) {
        if (x == 0 && y == 6)
            if (!(moved || dynamic_cast<Rook*>(board[0][7]) == nullptr || board[0][7]->moved || board[0][5] != nullptr || board[0][6] != nullptr)) return true;
        if (x == 0 && y == 2)
            if (!(moved || dynamic_cast<Rook*>(board[0][0]) == nullptr || board[0][0]->moved || board[0][3] != nullptr || board[0][2] != nullptr || board[0][1] != nullptr)) return true;
    } else if (pos.x == 7 && pos.y == 4) {
        if (x == 7 && y == 6)
            if (!(moved || color != 'b' || dynamic_cast<Rook*>(board[7][7]) == nullptr || board[7][7]->color != color || board[7][7]->moved || board[7][5] != nullptr || board[7][6] != nullptr)) return true;
        if (x == 7 && y == 2)
            if (!(moved || color != 'b' || dynamic_cast<Rook*>(board[7][0]) == nullptr || board[7][0]->color != color || board[7][0]->moved || board[7][3] != nullptr || board[7][2] != nullptr || board[7][1] != nullptr)) return true;
    }
    if (!(absstd(x - pos.x) <= 1 && absstd(y - pos.y) <= 1)) return false;
    if (board[x][y] != nullptr && board[x][y]->color == color) return false;
    return true;
}

char King::getChar() const { return color == 'w' ? 'K' : 'k'; }

bool Pawn::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    int coef = (color == 'w' ? 1 : -1), dx = x - pos.x;
    if (y - pos.y == 0) {
        if (coef == dx && board[x][y] == nullptr) return true;
        if (dx == 2*coef && !moved && board[pos.x + coef][y] == nullptr && board[x][y] == nullptr) return true;
        return false;
    }
    if (dx == coef && absstd(y - pos.y) == 1) {
        if (board[x][y] != nullptr && board[x][y]->color != color) return true;
        return false;
    }
    return false;
}

char Pawn::getChar() const { return color == 'w' ? 'P' : 'p'; }

bool Queen::isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const {
    if (x == pos.x && y == pos.y) return false;
    if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
    if (pos.x == x) {
        for (int i = minstd(pos.y, y)+1; i < maxstd(pos.y, y); i++)
            if (board[x][i] != nullptr) return false;
    } else if (pos.y == y) {
        for (int i = minstd(pos.x, x)+1; i < maxstd(pos.x, x); i++)
            if (board[i][y] != nullptr) return false;
    } else if (absstd(x - pos.x) == absstd(y - pos.y)) {
        int dx = (x > pos.x) ? 1 : -1, dy = (y > pos.y) ? 1 : -1;
        for (int i = 1; i < absstd(x - pos.x); i++)
            if (board[pos.x + dx*i][pos.y + dy*i] != nullptr) return false;
    } else return false;
    if (board[x][y] != nullptr && board[x][y]->color == color) return false;
    return true;
}

char Queen::getChar() const { return color == 'w' ? 'Q' : 'q'; }
Piece::Piece(char color, int x, int y) : color(color) {
    pos.x = x;
    pos.y = y;
    moved = false;
}

void Piece::setPosition(int x, int y) {
    pos.x = x;
    pos.y = y;
    moved = true;
}
Rook::Rook(char color, int x, int y) : Piece(color, x, y) {}
Knight::Knight(char color, int x, int y) : Piece(color, x, y) {}
Bishop::Bishop(char color, int x, int y) : Piece(color, x, y) {}
Queen::Queen(char color, int x, int y) : Piece(color, x, y) {}
King::King(char color, int x, int y) : Piece(color, x, y) {}
Pawn::Pawn(char color, int x, int y) : Piece(color, x, y) {}