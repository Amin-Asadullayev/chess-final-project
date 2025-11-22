#include "game.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "piece.h"
#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

Game::Game(int mode) : board(8, std::vector<Piece*>(8, nullptr)), turn('w'), mode(mode) {
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    logo = R"(
  .g8"""bgd `7MMF'  `7MMF'`7MM"""YMM   .M"""bgd  .M"""bgd                            
.dP'     `M   MM      MM    MM    `7  ,MI    "Y ,MI    "Y                            
dM'       `   MM      MM    MM   d    `MMb.     `MMb.          pd*"*b.     ,pP""Yq.  
MM            MMmmmmmmMM    MMmmMM      `YMMNq.   `YMMNq.     (O)   j8    6W'    `Wb 
MM.           MM      MM    MM   Y  , .     `MM .     `MM         ,;j9    8M      M8 
`Mb.     ,'   MM      MM    MM     ,M Mb     dM Mb     dM      ,-='    ,, YA.    ,A9 
  `"bmmmd'  .JMML.  .JMML..JMMmmmmMMM P"Ybmmd"  P"Ybmmd"      Ammmmmmm db  `Ybmmd9' 
)";
    // Initialize board pieces
    board[0][0] = new Rook('w', 0, 0);
    board[0][1] = new Knight('w', 0, 1);
    board[0][2] = new Bishop('w', 0, 2);
    board[0][3] = new Queen('w', 0, 3);
    board[0][4] = new King('w', 0, 4);
    board[0][5] = new Bishop('w', 0, 5);
    board[0][6] = new Knight('w', 0, 6);
    board[0][7] = new Rook('w', 0, 7);
    for(int j=0;j<8;j++) board[1][j] = new Pawn('w',1,j);
    board[7][0] = new Rook('b', 7, 0);
    board[7][1] = new Knight('b', 7, 1);
    board[7][2] = new Bishop('b', 7, 2);
    board[7][3] = new Queen('b', 7, 3);
    board[7][4] = new King('b', 7, 4);
    board[7][5] = new Bishop('b', 7, 5);
    board[7][6] = new Knight('b', 7, 6);
    board[7][7] = new Rook('b', 7, 7);
    for(int j=0;j<8;j++) board[6][j] = new Pawn('b',6,j);
}

void Game::print() {
    int sR, eR, rS, sC, eC, cS;
    if (this->getTurn() == 'w'){
        sR = 7; eR = -1; rS = -1; sC = 0; eC = 8; cS = 1;
    } else {
        sR = 0; eR = 8; rS = 1; sC = 7; eC = -1; cS = -1;
    }
    //Printing the logo
    SetConsoleTextAttribute(hcon, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout<<logo<<std::endl;
    SetConsoleTextAttribute(hcon, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    //Printing the board
    for (int i = sR; i != eR; i += rS) {
        std::cout << i + 1 << "|";
        for (int j = sC; j != eC; j += cS) {
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
    //Mirroring the board for the blacks
    std::cout<<(this->getTurn()=='w' ? "  a b c d e f g h" : "  h g f e d c b a")<<std::endl;
}

void Game::saveGameState(const std::string &filename) {
    std::ofstream out("games/"+filename, std::ios::app);
    if (!out) return;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] != nullptr) {
                out << board[i][j]->getChar() << board[i][j]->moved;
            } else {
                out << "  ";
            }
        }
        out << "\n";
    }
    out << "------\n";
}

void Game::loadGameState(const std::string& filename) {
    std::ifstream in("games/" + filename);
    if (!in) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    std::vector<std::vector<Piece*>> tempBoard(8, std::vector<Piece*>(8, nullptr));
    std::string line;
    int row = 0;
    int snapshotCount = 0;

    while (std::getline(in, line)) {
        if (line == "------") {
            snapshotCount++;
            row = 0;
            continue;
        }

        if (row >= 8) continue;

        for (int col = 0; col < 8; col++) {
            char c = ' ';
            char movedChar = '0';
            if (col * 2 < line.size()) c = line[col * 2];
            if (col * 2 + 1 < line.size()) movedChar = line[col * 2 + 1];
            bool movedFlag = (movedChar == '1');

            if (tempBoard[row][col] != nullptr) {
                delete tempBoard[row][col];
                tempBoard[row][col] = nullptr;
            }

            switch (c) {
                case 'P': tempBoard[row][col] = new Pawn('w', row, col); break;
                case 'p': tempBoard[row][col] = new Pawn('b', row, col); break;
                case 'R': tempBoard[row][col] = new Rook('w', row, col); break;
                case 'r': tempBoard[row][col] = new Rook('b', row, col); break;
                case 'N': tempBoard[row][col] = new Knight('w', row, col); break;
                case 'n': tempBoard[row][col] = new Knight('b', row, col); break;
                case 'B': tempBoard[row][col] = new Bishop('w', row, col); break;
                case 'b': tempBoard[row][col] = new Bishop('b', row, col); break;
                case 'Q': tempBoard[row][col] = new Queen('w', row, col); break;
                case 'q': tempBoard[row][col] = new Queen('b', row, col); break;
                case 'K': tempBoard[row][col] = new King('w', row, col); break;
                case 'k': tempBoard[row][col] = new King('b', row, col); break;
                case ' ': tempBoard[row][col] = nullptr; movedFlag = false; break;
            }

            if (tempBoard[row][col] != nullptr)
                tempBoard[row][col]->moved = movedFlag;
        }
        row++;
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] != nullptr) {
                delete board[i][j];
                board[i][j] = nullptr;
            }
            board[i][j] = tempBoard[i][j];
            tempBoard[i][j] = nullptr;
        }
    }
    turn = (snapshotCount % 2 == 0) ? 'w' : 'b';

    in.close();
}

bool Game::isCheck(char color){
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
                if (board[i][j]->isValid(kx, ky, board)) {
                    return true;
                };
            }
        }
    }
    return false;
}
char Game::getTurn(){
    return this->turn;
}
bool Game::isCheckMate(char color){
    if (!isCheck(color)) return false;
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
                            board[i][j]->setPosition(i, j);
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
bool Game::isStaleMate(char color){
    if (isCheck(color)) return false;
    return isCheckMate(color); 
}
bool Game::isPromAvail(int x, int y){
    if (dynamic_cast<Pawn*>(board[x][y]) != nullptr){
        if (board[x][y]->color == 'w' && x == 7 || board[x][y]->color == 'b' && x == 0) return true;
    }
    return false;
}
bool Game::promote(int x, int y, char opt, char color){
    switch (opt){
        case 'q':
            delete board[x][y];
            board[x][y] = new Queen(color, x, y);
            break;
        case 'Q':
            delete board[x][y];
            board[x][y] = new Queen(color, x, y);
            break;
        case 'r':
            delete board[x][y];
            board[x][y] = new Rook(color, x, y);
            break;
        case 'R':
            delete board[x][y];
            board[x][y] = new Rook(color, x, y);
            break;
        case 'b':
            delete board[x][y];
            board[x][y] = new Bishop(color, x, y);
            break;
        case 'B':
            delete board[x][y];
            board[x][y] = new Bishop(color, x, y);
            break;
        case 'n':
            delete board[x][y];
            board[x][y] = new Knight(color, x, y);
            break;
        case 'N':
            delete board[x][y];
            board[x][y] = new Knight(color, x, y);
            break;
        default:
            return false;
    }
    return true;
}
bool Game::isCheckAt(int x, int y, char color) {
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
bool Game::move(int x1, int y1, int x2, int y2){
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
