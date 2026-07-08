#pragma once

#include <vector>
#include <string>
#include "Guard.h"

using namespace std;

class Game
{
private:
    vector<string> board;
    vector<Guard> guards;
    vector<string> levelNames;

    int playerRow;
    int playerCol;
    bool gameOver;

    void moveGuards();
    bool positionOccupied(int row, int col);
    bool playerSeen();

    int showMenu();
    void loadLevel(int levelNumber);
    void printBoard();
    bool movePlayer(char input);

    bool isNumber(const string &text);
    string toUpper(const string &text);
    string trim(const string &text);

public:
    Game();
    void run();
};
