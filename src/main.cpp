#include <iostream>
#include <vector>

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

int abs(int a){
    return a >= 0 ? a : -a;
}

struct Position{
    int x; int y;
};

class Piece{
    public:
        Position pos;
        bool moved;
        char color;

        Piece(char color, int x, int y) : moved(false), color(color), pos{x, y} {}
        virtual void setPosition(int x, int y){
            pos.x = x;
            pos.y = y;
            moved = true;
        }

        virtual bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const = 0;
};

class Rook : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (pos.x == x){
                for (int i = min(pos.y, y)+1; i < max(pos.y, y); i++){
                    if (board[x][i] != nullptr) return false;
                }
            } else if (pos.y == y){
                for (int i = min(pos.x, x)+1; i < max(pos.x, x); i++){
                    if (board[i][y] != nullptr) return false;
                }
            } else {
                return false;
            }
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }
};

class Bishop : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (abs(x - pos.x) == abs(y - pos.y)){
                int dx = (x > pos.x) ? 1 : -1, dy = (y > pos.y) ? 1 : -1;
                for (int i = 1; i < abs(x - pos.x); i++){
                    if (board[pos.x + dx*i][pos.y + dy*i] != nullptr) return false;
                }
            } else {
                return false;
            }
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }
};

class Knight : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (!((abs(x - pos.x) == 2 && abs(y - pos.y) == 1) || (abs(x - pos.x) == 1 && abs(y - pos.y) == 2))) return false;
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }
};

class King : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (!(abs(x - pos.x) <= 1 && abs(y - pos.y) <= 1)) return false;
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }
};

class Pawn : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            int coef = this->color == 'w' ? 1 : -1, dx = x - pos.x;

            if(y - pos.y == 0){
                if (coef == dx && board[x][y] == nullptr) return true;
                if (dx == 2*coef && !moved && board[pos.x + coef][y] == nullptr && board[x][y] == nullptr) return true;
                return false;
            }
            if (dx == coef && abs(y - pos.y) == 1){
                if (board[x][y] != nullptr && board[x][y]->color != this->color) return true;
                return false;
            }
            return false;
        }
};

class Queen : Piece{
    public:
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (pos.x == x){
                for (int i = min(pos.y, y)+1; i < max(pos.y, y); i++){
                    if (board[x][i] != nullptr) return false;
                }
            } else if (pos.y == y){
                for (int i = min(pos.x, x)+1; i < max(pos.x, x); i++){
                    if (board[i][y] != nullptr) return false;
                }
            } else if (abs(x - pos.x) == abs(y - pos.y)){
                int dx = (x > pos.x) ? 1 : -1, dy = (y > pos.y) ? 1 : -1;
                for (int i = 1; i < abs(x - pos.x); i++){
                    if (board[pos.x + dx*i][pos.y + dy*i] != nullptr) return false;
                }
            } else {
                return false;
            }
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }
};

int main(){

    return 0;
}