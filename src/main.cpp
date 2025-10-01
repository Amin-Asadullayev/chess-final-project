#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

int abs(int a){
    return a >= 0 ? a : -a;
}

int convert(char a){
    int b;
    switch(a){
        case 'a':
            b = 0;
            break;
        case 'b':
            b = 1;
            break;
        case 'c':
            b = 2;
            break;
        case 'd':
            b = 3;
            break;
        case 'e':
            b = 4;
            break;
        case 'f':
            b = 5;
            break;
        case 'g':
            b = 6;
            break;
        case 'h':
            b = 7;
            break;
        default:
            b = 8;
    }
    return b;
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
        virtual char getChar() const = 0;
};

class Rook : public Piece{
    public:
        Rook(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
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

        char getChar() const override {
            return this->color == 'w' ? 'R' : 'r';
        }
};

class Bishop : public Piece{
    public:
        Bishop(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
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

        char getChar() const override {
            return this->color == 'w' ? 'B' : 'b';
        }
};

class Knight : public Piece{
    public:
        Knight(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (!((abs(x - pos.x) * abs(y - pos.y) == 2))) return false;
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }

        char getChar() const override {
            return this->color == 'w' ? 'N' : 'n';
        }
};

class King : public Piece{
    public:
        King(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
            if (x < 0 || x >= 8 || y < 0 || y >= 8) return false;
            if (pos.x == 0 && pos.y == 4){
                if (x == 0 && y == 6){
                    if (!(this->moved || dynamic_cast<Rook*>(board[0][7]) == nullptr || board[0][7]->moved || board[0][5] != nullptr || board[0][6] != nullptr)) return true;
                } else if (x == 0 && y == 2){
                    if (!(this->moved || dynamic_cast<Rook*>(board[0][0]) == nullptr || board[0][0]->moved || board[0][3] != nullptr || board[0][2] != nullptr || board[0][1] != nullptr)) return true;
                }
            } else if (pos.x == 7 && pos.y == 4){
                if (x == 7 && y == 6){
                    if (!(this->moved || this->color != 'b' || dynamic_cast<Rook*>(board[7][7]) == nullptr || board[7][7]->color != this->color || board[7][7]->moved || board[7][5] != nullptr || board[7][6] != nullptr)) return true;
                } else if (x == 7 && y == 2){
                    if (!(this->moved || this->color != 'b' || dynamic_cast<Rook*>(board[7][0]) == nullptr || board[7][0]->color != this->color || board[7][0]->moved || board[7][3] != nullptr || board[7][2] != nullptr || board[7][1] != nullptr)) return true;
                }
            }
            if (!(abs(x - pos.x) <= 1 && abs(y - pos.y) <= 1)) return false;
            if (board[x][y] != nullptr && board[x][y]->color == this->color) return false;
            return true;
        }

        char getChar() const override {
            return this->color == 'w' ? 'K' : 'k';
        }
};

class Pawn : public Piece{
    public:
        Pawn(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
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

        char getChar() const override {
            return this->color == 'w' ? 'P' : 'p';
        }
};

class Queen : public Piece{
    public:
        Queen(char color, int x, int y) : Piece(color, x, y) {}
        bool isValid(int x, int y, const std::vector<std::vector<Piece*>> &board) const override {
            if (x == pos.x && y == pos.y) return false;
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

        char getChar() const override {
            return this->color == 'w' ? 'Q' : 'q';
        }
};

class Game{
    std::vector<std::vector<Piece*>> board{8, std::vector<Piece*>(8, nullptr)};
    char turn;

    public:
        HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
        Game(){
            turn = 'w';
            board[0][0] = new Rook('w', 0, 0);
            board[0][1] = new Knight('w', 0, 1);
            board[0][2] = new Bishop('w', 0, 2);
            board[0][3] = new Queen('w', 0, 3);
            board[0][4] = new King('w', 0, 4);
            board[0][5] = new Bishop('w', 0, 5);
            board[0][6] = new Knight('w', 0, 6);
            board[0][7] = new Rook('w', 0, 7);
            board[1][0] = new Pawn('w', 1, 0);
            board[1][1] = new Pawn('w', 1, 1);
            board[1][2] = new Pawn('w', 1, 2);
            board[1][3] = new Pawn('w', 1, 3);
            board[1][4] = new Pawn('w', 1, 4);
            board[1][5] = new Pawn('w', 1, 5);
            board[1][6] = new Pawn('w', 1, 6);
            board[1][7] = new Pawn('w', 1, 7);
            board[7][0] = new Rook('b', 7, 0);
            board[7][1] = new Knight('b', 7, 1);
            board[7][2] = new Bishop('b', 7, 2);
            board[7][3] = new Queen('b', 7, 3);
            board[7][4] = new King('b', 7, 4);
            board[7][5] = new Bishop('b', 7, 5);
            board[7][6] = new Knight('b', 7, 6);
            board[7][7] = new Rook('b', 7, 7);
            board[6][0] = new Pawn('b', 6, 0);
            board[6][1] = new Pawn('b', 6, 1);
            board[6][2] = new Pawn('b', 6, 2);
            board[6][3] = new Pawn('b', 6, 3);
            board[6][4] = new Pawn('b', 6, 4);
            board[6][5] = new Pawn('b', 6, 5);
            board[6][6] = new Pawn('b', 6, 6);
            board[6][7] = new Pawn('b', 6, 7);
        }

        bool isCheck(char color){
            char target = color == 'w' ? 'b' : 'w';
            int kx, ky;
            bool exitLoop = false;
            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                    if (board[i][j] != nullptr && dynamic_cast<King*>(board[i][j]) != nullptr && board[i][j]->color == color){
                        kx = board[i][j]->pos.x;
                        ky = board[i][j]->pos.y;
                        exitLoop = true;
                        break;
                    }
                }
                if (exitLoop) break;
            }

            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                    if (board[i][j] != nullptr && board[i][j]->color == target){
                        if (board[i][j]->isValid(kx, ky, board)) return true;
                    }
                }
            }
            return false;
        }

        void print(){
            for (int i = 7; i >= 0; i--) {
                std::cout << i + 1 << "|";
                for (int j = 0; j < 8; j++) {
                    WORD bgColor = (i+j) % 2 == 0 ? BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE : BACKGROUND_INTENSITY;
                    if (board[i][j] != nullptr) {
                        WORD fgColor = (board[i][j]->color == 'w') ? FOREGROUND_GREEN | FOREGROUND_RED: 0;
                        SetConsoleTextAttribute(hcon, fgColor | bgColor);
                        std::cout << board[i][j]->getChar() << " ";
                    } else {
                        SetConsoleTextAttribute(hcon, bgColor);
                        std::cout << "  ";
                    }
                }
                SetConsoleTextAttribute(hcon, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << std::endl;
            }
            std::cout<<"  _______________"<<std::endl;
            std::cout<<"  a b c d e f g h"<<std::endl;
        }

        char getTurn(){
            return this->turn;
        }

        bool isCheckMate(char color){
            for (int i = 0; i < 8; i++){
                for (int j = 0; j < 8; j++){
                    if (board[i][j] != nullptr && board[i][j]->color == color){
                        for (int a = 0; a < 8; a++){
                            for (int b = 0; b < 8; b++){
                                if (board[i][j]->isValid(a, b, board)){
                                    bool nullflag = board[a][b] != nullptr;
                                    Piece* tempto = board[a][b];
                                    bool movedto = nullflag ? board[a][b]->moved : false;
                                    Piece* tempfrom = board[i][j];
                                    bool movedfrom = board[i][j]->moved;
                                    board[a][b] = board[i][j];
                                    board[i][j] = nullptr;
                                    board[a][b]->setPosition(a, b);
                                    bool stillInCheck = isCheck(color);
                                    board[i][j] = tempfrom;
                                    board[i][j]->moved = movedfrom;
                                    board[a][b] = tempto;
                                    if (nullflag) board[a][b]->moved = movedto;
                                    if (!stillInCheck) return false;
                                        }
                                    }
                        }
                    }
                }
            }
            return true;
        }

        bool isCheckAt(int x, int y, char color) {
            char opp = (color == 'w') ? 'b' : 'w';
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (board[i][j] != nullptr && board[i][j]->color == opp) {
                        if (board[i][j]->isValid(x, y, board)) return true;
                    }
                }
            }
            return false;
        }


        bool move(int x1, int y1, int x2, int y2){
            if (board[x1][y1] == nullptr || board[x1][y1]->color != this->turn) return false;
            if (dynamic_cast<King*>(board[x1][y1]) != nullptr && (((x1 == 0 && y1 == 4) && (x2 == 0 && (y2 == 6 || y2 == 2))) || ((x1 == 7 && y1 == 4) && (x2 == 7 && (y2 == 6 || y2 == 2))))) {
                if (isCheck(this->turn)) return false;
                if (x1 == 0 && y1 == 4 && x2 == 0 && y2 == 6 && isCheckAt(0, 5, this->turn)) return false;
                if (x1 == 0 && y1 == 4 && x2 == 0 && y2 == 2 && isCheckAt(0, 3, this->turn)) return false;
                if (x1 == 7 && y1 == 4 && x2 == 7 && y2 == 6 && isCheckAt(7, 5, this->turn)) return false;
                if (x1 == 7 && y1 == 4 && x2 == 7 && y2 == 2 && isCheckAt(7, 3, this->turn)) return false;
                if (x1 == 0 && y1 == 4 && y2 == 6) {
                    board[0][5] = board[0][7];
                    board[0][5]->setPosition(0,5);
                    board[0][7] = nullptr;
                } else if (x1 == 0 && y1 == 4 && y2 == 2) {
                    board[0][3] = board[0][0];
                    board[0][3]->setPosition(0,3);
                    board[0][0] = nullptr;
                } else if (x1 == 7 && y1 == 4 && y2 == 6) {
                    board[7][5] = board[7][7];
                    board[7][5]->setPosition(7,5);
                    board[7][7] = nullptr;
                } else if (x1 == 7 && y1 == 4 && y2 == 2) {
                    board[7][3] = board[7][0];
                    board[7][3]->setPosition(7,3);
                    board[7][0] = nullptr;
                }
                board[x2][y2] = board[x1][y1];
                board[x1][y1] = nullptr;
                board[x2][y2]->setPosition(x2, y2);
                turn = turn == 'w' ? 'b' : 'w';
                return true;
            }
            if (board[x1][y1]->isValid(x2, y2, board)){
                Piece* tempto = board[x2][y2];
                bool nullflag = tempto != nullptr;
                bool movedto = nullflag ? tempto->moved : false;
                Piece* tempfrom = board[x1][y1];
                bool movedfrom = board[x1][y1]->moved;
                board[x2][y2] = board[x1][y1];
                board[x1][y1] = nullptr; 
                board[x2][y2]->setPosition(x2, y2);
                if(isCheck(this->turn)){
                    board[x1][y1] = tempfrom;
                    board[x1][y1]->moved = movedfrom;
                    board[x2][y2] = tempto;
                    if (nullflag) board[x2][y2]->moved = movedto;
                    return false;
                }
                if (nullflag) delete tempto;
                turn = turn == 'w' ? 'b' : 'w'; 
                return true;
            } 
            return false;
        }
};

int main(){
    Game newGame = Game{};
    std::string from, to;
    bool flag = false;
    while (true){
        system("cls");
        newGame.print();
        if (flag) std::cout<<"Invalid move!"<<std::endl;
        std::cout<<"Enter your move: ";
        std::cin>>from>>to;
        std::transform(from.begin(), from.end(), from.begin(), ::tolower);
        std::transform(to.begin(), to.end(), to.begin(), ::tolower);
        if (from == "exit") return 0;
        if (from.length() != 2 || to.length() != 2) {
            flag = true;
            continue;
        } else {
            flag = false;
            if (from[0] > 'h' || from[0] < 'a' || to[0] > 'h' || to[0] < 'a' || from[1] > '8' || from[1] < '1' || to[1] > '8' || to[1] < '1' ){
                flag = true;
                continue;
            }
            int y1 = convert(from[0]), y2 = convert(to[0]), x1 = from[1] - '1', x2 = to[1] - '1';
            if(!newGame.move(x1, y1, x2, y2)) {flag = true;continue;}
            if (newGame.isCheckMate(newGame.getTurn())) break;
        }        
    }
    system("cls");
    newGame.print();
    std::string thewin = (newGame.getTurn() == 'w' ? "Black" : "White");
    std::cout<<thewin<<" has won the game by checkmate!"<<std::endl;
    return 0;
}