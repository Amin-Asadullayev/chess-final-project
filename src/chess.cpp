#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include "utils.h"
#include "piece.h"
#include "game.h"
#include <conio.h>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <filesystem>

//Menu for selecting a saved game
std::string chooseSavedGame() {
    namespace fs = std::filesystem;
    std::vector<std::string> savedGames;

    for (const auto& entry : fs::directory_iterator("games")) {
        std::string fname = entry.path().filename().string();
        if (fname.size() > 10 && fname.substr(fname.size() - 10) == "_state.txt") {
            savedGames.push_back(fname);
        }
    }

    if (savedGames.empty()) {
        std::cout << "No saved games found.\n";
        std::cout << "Press any key to return to the main menu...";
        _getch();
        return "";
    }

    int choice = 0;
    char ch;
    while (true) {
        system("cls");
        std::cout << "Select a saved game:\n";
        for (int i = 0; i < savedGames.size(); i++) {
            if (i == choice) std::cout << "> ";
            else std::cout << "  ";
            std::cout << savedGames[i] << "\n";
        }

        ch = _getch();
        if (ch == 13) break;
        else if (ch == -32 || ch == 0) {
            ch = _getch();
            if (ch == 72) choice = (choice + savedGames.size() - 1) % savedGames.size();
            else if (ch == 80) choice = (choice + 1) % savedGames.size();
        }
    }

    return savedGames[choice];
}


int main() {
    while (true){
        //Main menu
        std::filesystem::create_directory("games");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        int mode = 0;
        char ch;
        std::string logo = R"(
    .g8"""bgd `7MMF'  `7MMF'`7MM"""YMM   .M"""bgd  .M"""bgd                            
    .dP'     `M   MM      MM    MM    `7  ,MI    "Y ,MI    "Y                            
    dM'       `   MM      MM    MM   d    `MMb.     `MMb.          pd*"*b.     ,pP""Yq.  
    MM            MMmmmmmmMM    MMmmMM      `YMMNq.   `YMMNq.     (O)   j8    6W'    `Wb 
    MM.           MM      MM    MM   Y  , .     `MM .     `MM         ,;j9    8M      M8 
    `Mb.     ,'   MM      MM    MM     ,M Mb     dM Mb     dM      ,-='    ,, YA.    ,A9 
    `"bmmmd'  .JMML.  .JMML..JMMmmmmMMM P"Ybmmd"  P"Ybmmd"      Ammmmmmm db  `Ybmmd9' 
    )";
        std::string menu[] = {"New game", "Load game", "Quit"};
        system("cls");
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        std::cout<<logo<<std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << "Use arrow keys to navigate and enter to choose:\n";
        for (int i = 0; i < 3; i++){
            if(mode==i){
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                std::cout<<"* ";
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            } else std::cout<<"  ";
            std::cout<<menu[i]<<std::endl;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }

        //Take up and down arrow as input
        while (true) {
            if(_kbhit()){
            ch = _getch();
            if (int(ch) == 13) break;
            else if (int(ch) == -32 or int(ch) == 0){
                system("cls");
                ch = _getch();
                if (ch == 72) mode = (mode+2)%3;
                else if (ch == 80) mode = (mode+1)%3;
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout<<logo<<std::endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                std::cout << "Use arrow keys to navigate and enter to choose:\n";
                for (int i = 0; i < 3; i++){
                    if(mode==i){
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        std::cout<<"* ";
                        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    } else std::cout<<"  ";
                    std::cout<<menu[i]<<std::endl;
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                }
            } 
        }
        }
        if (mode == 2) return 0;

        //Initialize a new game and its save files
        Game newGame(0);
        std::string fname = generateGameFilename();
        std::string moveHistoryFile = fname + "_moves.txt";
        std::string gameStateFile = fname + "_state.txt";
        std::string from, to;
        if (mode == 1) {
            try{
                std::string chosenFile = chooseSavedGame();
                if (chosenFile.empty()) continue;
                newGame.loadGameState(chosenFile);
                moveHistoryFile = chosenFile;
                gameStateFile = chosenFile;
            } catch (std::runtime_error &e){
                std::cout<<"ERROR: "<<e.what()<<std::endl;
                _getch();
                continue;
            }
        }
        bool flag = false;

        while (true) {
            system("cls");
            newGame.print();

            if (flag) std::cout << "Invalid move!" << std::endl;

            std::cout << "Enter your move: ";
            std::cin >> from >> to;
            std::transform(from.begin(), from.end(), from.begin(), ::tolower);
            std::transform(to.begin(), to.end(), to.begin(), ::tolower);

            if (from == "exit" && to == "game") break;

            //Moves to coordinates
            if (from.length() != 2 || to.length() != 2) {
                flag = true;
                continue;
            } else {
                flag = false;
                if (from[0] > 'h' || from[0] < 'a' || to[0] > 'h' || to[0] < 'a' || from[1] > '8' || from[1] < '1' || to[1] > '8' || to[1] < '1') {
                    flag = true;
                    continue;
                }

                int y1 = convert(from[0]), y2 = convert(to[0]);
                int x1 = from[1] - '1', x2 = to[1] - '1';

                //Checking if the move is legal
                if (!newGame.move(x1, y1, x2, y2)) {
                    flag = true;
                    continue;
                } else {
                    try{
                        newGame.saveGameState(gameStateFile);
                        std::ofstream moveOut("games/" + moveHistoryFile, std::ios::app);
                        if(!moveOut.good()) throw std::runtime_error("Could not save the game!");
                        if (moveOut) moveOut << from << " " << to << std::endl;
                        moveOut.close();
                    } catch (const std::runtime_error &err){
                        std::cout<<"ERROR: "<<err.what()<<std::endl;
                    }
                }

                if (newGame.isPromAvail(x2, y2)) {
                    char dest;
                    while (true) {
                        system("cls");
                        newGame.print();
                        std::cout << "Promote pawn to (Q/R/B/N): ";
                        std::cin >> dest;
                        if (newGame.promote(x2, y2, dest, newGame.getTurn() == 'w' ? 'b' : 'w')) break;
                    }
                    newGame.saveGameState(gameStateFile);
                }

                //If checkmate or stalemate, end the game
                if (newGame.isCheckMate(newGame.getTurn())) {
                    system("cls");
                    newGame.print();
                    std::string thewin = (newGame.getTurn() == 'w' ? "Black" : "White");
                    std::cout << thewin << " has won the game by checkmate!" << std::endl;
                    std::cout<<"Press any key to return to the main menu...";
                    _getch();
                    break;
                } else if (newGame.isStaleMate(newGame.getTurn())) {
                    system("cls");
                    newGame.print();
                    std::cout << "The game ends in stalemate!" << std::endl;
                    std::cout<<"Press any key to return to the main menu...";
                    _getch();
                    break;
                }
            }
        }
    }

    return 0;
}
