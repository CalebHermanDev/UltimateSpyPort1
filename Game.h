#pragma once

#include <vector>
#include <string>
#include "Guard.h"
#include "Door.h"
#include "Switch.h"

using namespace std;

class Game
{
private:
    vector<string> board;
    vector<Guard> guards;
    vector<Door> doors;
    vector<Switch> switches;
    vector<string> levelNames;
    string currentLevelName;

    int playerRow;
    int playerCol;
    bool gameOver;

    void moveGuards();
    bool positionOccupied(int row, int col);
    bool playerSeen();

    int showMenu();
    void loadLevel(int levelNumber);
    bool loadLevelFromFile(const string &fileName);
    void printBoard();
    void printEditorBoard() const;
    bool movePlayer(char input);
    void handleInspect();
    void triggerSwitches(int row, int col);

    void createNewLevel();
    void editLevel(const string &levelName, int rows, int cols);
    void editExistingLevel();
    void runEditor();
    void resizeLevel();
    void renameLevel();
    void handleWallPlacement();
    bool readTile(int &row, int &col);
    bool saveLevelToFile(const string &levelName) const;
    void addToLevelNames(const string &name);
    void clearObjectsAt(int row, int col);
    void placeWall(int row, int col);
    void placeEmpty(int row, int col);
    void placeGoal(int row, int col);
    void placePlayer(int row, int col);
    void placeGuard(int row, int col);
    void placeDoor(int row, int col);
    void placeSwitch(int row, int col);
    void inspectTile(int row, int col) const;
    bool isValidTile(int row, int col) const;
    bool isNumber(const string &text) const;
    string getLevelDirectory() const;
    string getLevelFilePath(const string &levelName) const;
    string stripLvlExtension(const string &name) const;
    string toUpper(const string &text) const;
    string trim(const string &text) const;

public:
    Game();
    void run();
};
