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

    int playerRow;
    int playerCol;
    bool gameOver;
    void moveGuards();
    bool positionOccupied(int row, int col);
    bool playerSeen();

    void showMenu();
    void loadLevel(int levelNumber);
    void findPlayer();
    void printBoard();
    void movePlayer(char input);

public:
    void run();
};