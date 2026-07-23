#include "Game.h"
#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace std;

Game::Game()
{
    gameOver = false;
    playerRow = -1;
    playerCol = -1;
    levelNames.push_back("A New Enemy");
    levelNames.push_back("The Truth of the Weapon");
    levelNames.push_back("Patrolling Rectangle");
    levelNames.push_back("Complex Patrol");
    levelNames.push_back("Multi-Group Doors");
    levelNames.push_back("Switch Trap");
}

void Game::run()
{
    bool quit = false;

    while (!quit)
    {
        int levelChoice = showMenu();

        if (levelChoice == 0)
        {
            quit = true;
            break;
        }

        if (levelChoice == -1)
        {
            createNewLevel();
            continue;
        }

        if (levelChoice == -3)
        {
            editExistingLevel();
            continue;
        }

        if (levelChoice == -2)
        {
            gameOver = false;
        }
        else
        {
            loadLevel(levelChoice);
            gameOver = false;
        }

        while (!gameOver)
        {
            printBoard();
            cout << "\nMove (WASD), Inspect (I), or Q to quit: ";

            string line;
            getline(cin, line);

            if (!cin)
            {
                quit = true;
                break;
            }

            line = trim(line);

            if (line.size() != 1)
            {
                cout << "\nInvalid input. Please enter W, A, S, D, I, or Q.\n";
                continue;
            }

            char input = toupper(line[0]);

            if (input != 'W' && input != 'A' && input != 'S' &&
                input != 'D' && input != 'Q' && input != 'I')
            {
                cout << "\nInvalid input. Please enter W, A, S, D, I, or Q.\n";
                continue;
            }

            if (input == 'Q')
            {
                quit = true;
                break;
            }

            if (input == 'I')
            {
                handleInspect();
                continue;
            }

            bool moved = movePlayer(input);

            if (gameOver)
            {
                break;
            }

            if (!moved)
            {
                continue;
            }

            moveGuards();

            if (!gameOver && playerSeen())
            {
                printBoard();
                cout << "\nYou were spotted!\n";
                gameOver = true;
            }
        }

        if (quit)
        {
            break;
        }

        cout << "\nPress ENTER to return to the menu...";
        string dummy;
        getline(cin, dummy);
    }

    cout << "\nThanks for playing Ultra-Spy!\n";
}

int Game::showMenu()
{
    while (true)
    {
        cout << "\nWelcome to Ultra-Spy!\n";
        cout << "Select a level:\n";

        for (int i = 0; i < levelNames.size(); i++)
        {
            cout << " " << (i + 1) << ") " << levelNames[i] << "\n";
        }

        cout << "\nN) Create a new level";
        cout << "\nE) Edit an existing level";
        cout << "\nL) Load a level from a file";
        cout << "\nEnter a level number or name (or Q to quit): ";

        string line;
        getline(cin, line);

        if (!cin)
        {
            return 0;
        }

        line = trim(line);
        string upperLine = toUpper(line);

        if (upperLine == "Q")
        {
            return 0;
        }

        if (upperLine == "N" || upperLine == "NEW" || upperLine == "CREATE")
        {
            return -1;
        }

        if (upperLine == "E" || upperLine == "EDIT")
        {
            return -3;
        }

        if (upperLine == "L" || upperLine == "LOAD")
        {
            cout << "Enter a level file name (without .lvl if you want): ";
            string fileName;
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty())
            {
                cout << "\nInvalid file name.\n";
                continue;
            }

            if (loadLevelFromFile(fileName))
            {
                return -2;
            }

            continue;
        }

        if (isNumber(line))
        {
            int choice = stoi(line);

            if (choice >= 1 && choice <= (int)levelNames.size())
            {
                return choice;
            }
        }
        else
        {
            for (int i = 0; i < levelNames.size(); i++)
            {
                if (toUpper(levelNames[i]) == upperLine)
                {
                    return i + 1;
                }
            }

            if (loadLevelFromFile(line))
            {
                return -2;
            }
        }

        cout << "\nInvalid selection. Please try again.\n";
    }
}

bool Game::isNumber(const string &text) const
{
    if (text.empty())
        return false;

    for (int i = 0; i < text.size(); i++)
    {
        if (!isdigit(text[i]))
            return false;
    }

    return true;
}

string Game::toUpper(const string &text) const
{
    string result = text;

    for (int i = 0; i < result.size(); i++)
    {
        result[i] = toupper(result[i]);
    }

    return result;
}

string Game::trim(const string &text) const
{
    int start = 0;
    int end = (int)text.size() - 1;

    while (start <= end && isspace(text[start]))
        start++;

    while (end >= start && isspace(text[end]))
        end--;

    if (start > end)
        return "";

    return text.substr(start, end - start + 1);
}

void Game::loadLevel(int levelNumber)
{
    if (levelNumber > 6 && levelNumber <= (int)levelNames.size())
    {
        loadLevelFromFile(levelNames[levelNumber - 1]);
        return;
    }

    guards.clear();
    doors.clear();
    switches.clear();

    if (levelNumber == 1)
    {
        board =
            {
                "########",
                "#      #",
                "#   #  #",
                "#   #  #",
                "#   # $#",
                "########"};

        playerRow = 4;
        playerCol = 1;

        guards.push_back(Guard(1, 5, 'v'));
        guards.push_back(Guard(2, 1, '>'));
    }
    else if (levelNumber == 2)
    {
        board =
            {
                "###########",
                "#     #   #",
                "# ### # # #",
                "# #     # #",
                "# # ###   #",
                "#       $ #",
                "###########"};

        playerRow = 1;
        playerCol = 1;

        guards.push_back(Guard(1, 8, 'v'));
        guards.push_back(Guard(3, 5, '>'));
        guards.push_back(Guard(5, 3, '^'));
    }
    else if (levelNumber == 3)
    {
        board =
            {
                "#############",
                "#     #   $ #",
                "#     #     #",
                "#           #",
                "#     #     #",
                "#############"};

        playerRow = 2;
        playerCol = 2;

        guards.push_back(Guard(1, 5, '>', true));
    }
    else if (levelNumber == 4)
    {
        board =
            {
                "###############",
                "#       #     #",
                "##    # #     #",
                "#     # #     #",
                "#  #          #",
                "#  ####  ######",
                "#   #         #",
                "#   #   ####  #",
                "#   #      # $#",
                "###############"};

        playerRow = 2;
        playerCol = 2;

        guards.push_back(Guard(2, 11, '<', true));
    }
    else if (levelNumber == 5)
    {
        board =
            {
                "################",
                "#             $#",
                "#              #",
                "#              #",
                "#   #  #  ######",
                "#      #       #",
                "################"};

        playerRow = 5;
        playerCol = 1;

        switches.push_back(Switch(5, 5, 0));
        doors.push_back(Door(1, 7, 0));
        doors.push_back(Door(2, 7, 0));
        doors.push_back(Door(3, 7, 0));

        switches.push_back(Switch(5, 14, 1));
        doors.push_back(Door(1, 12, 1));
        doors.push_back(Door(2, 12, 1));
        doors.push_back(Door(3, 12, 1));

        guards.push_back(Guard(4, 3, '^'));
    }
    else if (levelNumber == 6)
    {
        board =
            {
                "########################",
                "#                  # $ #",
                "#     ######       #   #",
                "#     #            #   #",
                "#     #                #",
                "#     #                #",
                "#     #    #       #   #",
                "########################"};

        playerRow = 1;
        playerCol = 1;

        switches.push_back(Switch(5, 5, 0));
        doors.push_back(Door(1, 6, 0));

        switches.push_back(Switch(6, 18, 1));
        doors.push_back(Door(3, 11, 1));
        doors.push_back(Door(4, 11, 1));
        doors.push_back(Door(5, 11, 1));

        switches.push_back(Switch(6, 8, 2));
        doors.push_back(Door(4, 19, 2));
        doors.push_back(Door(5, 19, 2));

        guards.push_back(Guard(1, 5, 'v'));
        guards.push_back(Guard(1, 13, '>', true));
    }
}

bool Game::loadLevelFromFile(const string &fileName)
{
    string baseName = stripLvlExtension(fileName);
    if (baseName.empty())
    {
        cout << "\nInvalid level name.\n";
        return false;
    }

    string path = getLevelFilePath(baseName);
    ifstream input(path);
    if (!input.is_open())
    {
        cout << "\nNo saved level named '" << baseName << "' was found.\n";
        return false;
    }

    string header;
    getline(input, header);
    header = trim(header);
    if (header.empty())
    {
        cout << "\nThe level file is empty.\n";
        return false;
    }

    istringstream headerStream(header);
    int rows = 0;
    int cols = 0;
    headerStream >> rows >> cols;

    if (rows <= 0 || cols <= 0)
    {
        cout << "\nThe level file is invalid.\n";
        return false;
    }

    guards.clear();
    doors.clear();
    switches.clear();
    board.assign(rows, string(cols, ' '));
    playerRow = -1;
    playerCol = -1;
    currentLevelName = baseName;

    string line;
    while (getline(input, line))
    {
        line = trim(line);
        if (line.empty())
        {
            continue;
        }

        istringstream stream(line);
        string type;
        stream >> type;
        type = toUpper(type);

        int row = 0;
        int col = 0;
        if (!(stream >> row >> col))
        {
            continue;
        }

        if (type == "WALL")
        {
            if (isValidTile(row, col))
            {
                board[row][col] = '#';
            }
        }
        else if (type == "GOAL")
        {
            if (isValidTile(row, col))
            {
                board[row][col] = '$';
            }
        }
        else if (type == "EMPTY")
        {
            if (isValidTile(row, col))
            {
                board[row][col] = ' ';
                clearObjectsAt(row, col);
            }
        }
        else if (type == "PLAYER")
        {
            if (isValidTile(row, col))
            {
                playerRow = row;
                playerCol = col;
                board[row][col] = ' ';
            }
        }
        else if (type == "GUARD")
        {
            if (isValidTile(row, col))
            {
                string movement;
                string direction;
                if (stream >> movement >> direction)
                {
                    clearObjectsAt(row, col);
                    board[row][col] = ' ';
                    guards.push_back(Guard(row, col, toupper(direction[0]), toUpper(movement) == "PATROLLING"));
                }
            }
        }
        else if (type == "DOOR")
        {
            if (isValidTile(row, col))
            {
                int groupId = 0;
                if (stream >> groupId)
                {
                    clearObjectsAt(row, col);
                    board[row][col] = ' ';
                    doors.push_back(Door(row, col, groupId));
                }
            }
        }
        else if (type == "SWITCH")
        {
            if (isValidTile(row, col))
            {
                int groupId = 0;
                if (stream >> groupId)
                {
                    clearObjectsAt(row, col);
                    board[row][col] = ' ';
                    switches.push_back(Switch(row, col, groupId));
                }
            }
        }
    }

    addToLevelNames(baseName);
    cout << "\nLoaded level '" << baseName << "'.\n";
    return true;
}

void Game::printBoard()
{
    vector<string> tempBoard = board;

    for (int i = 0; i < doors.size(); i++)
    {
        if (!doors[i].getIsOpen())
        {
            tempBoard[doors[i].getRow()][doors[i].getCol()] = 'D';
        }
    }

    for (int i = 0; i < switches.size(); i++)
    {
        tempBoard[switches[i].getRow()][switches[i].getCol()] = 'S';
    }

    for (int i = 0; i < guards.size(); i++)
    {
        tempBoard[guards[i].getRow()][guards[i].getCol()] = guards[i].getDirection();
    }

    if (playerRow >= 0 && playerCol >= 0)
    {
        tempBoard[playerRow][playerCol] = '@';
    }

    cout << endl;

    for (string row : tempBoard)
    {
        cout << row << endl;
    }
}

void Game::printEditorBoard() const
{
    cout << "\nCurrent level: " << currentLevelName << "\n";
    cout << "    ";
    for (int col = 0; col < (int)board[0].size(); col++)
    {
        cout << col % 10 << " ";
    }
    cout << "\n";

    for (int row = 0; row < (int)board.size(); row++)
    {
        cout << row << "   ";
        for (int col = 0; col < (int)board[row].size(); col++)
        {
            char symbol = board[row][col];
            if (row == playerRow && col == playerCol)
            {
                symbol = '@';
            }
            else
            {
                for (int i = 0; i < guards.size(); i++)
                {
                    if (guards[i].getRow() == row && guards[i].getCol() == col)
                    {
                        symbol = guards[i].getDirection();
                        break;
                    }
                }
                if (symbol == ' ' || symbol == '#')
                {
                    for (int i = 0; i < doors.size(); i++)
                    {
                        if (doors[i].getRow() == row && doors[i].getCol() == col)
                        {
                            symbol = 'D';
                            break;
                        }
                    }
                }
                if (symbol == ' ' || symbol == '#')
                {
                    for (int i = 0; i < switches.size(); i++)
                    {
                        if (switches[i].getRow() == row && switches[i].getCol() == col)
                        {
                            symbol = 'S';
                            break;
                        }
                    }
                }
            }

            if (symbol == ' ')
            {
                symbol = '.';
            }
            cout << symbol << " ";
        }
        cout << "\n";
    }
}

bool Game::positionOccupied(int row, int col)
{
    if (row < 0 || row >= (int)board.size() ||
        col < 0 || col >= (int)board[row].size())
        return true;

    if (board[row][col] == '#')
        return true;

    if (board[row][col] == '$')
        return true;

    for (int i = 0; i < doors.size(); i++)
    {
        if (doors[i].getRow() == row && doors[i].getCol() == col && !doors[i].getIsOpen())
        {
            return true;
        }
    }

    if (row == playerRow && col == playerCol)
        return true;

    for (int i = 0; i < guards.size(); i++)
    {
        if (guards[i].getRow() == row && guards[i].getCol() == col)
        {
            return true;
        }
    }

    return false;
}

bool Game::movePlayer(char input)
{
    int newRow = playerRow;
    int newCol = playerCol;

    if (input == 'W')
        newRow--;
    else if (input == 'S')
        newRow++;
    else if (input == 'A')
        newCol--;
    else if (input == 'D')
        newCol++;

    if (newRow < 0 || newRow >= (int)board.size() ||
        newCol < 0 || newCol >= (int)board[newRow].size())
    {
        cout << "\nYou can't move there!\n";
        return false;
    }

    if (board[newRow][newCol] == '#')
    {
        cout << "\nYou can't walk through walls!\n";
        return false;
    }

    for (int i = 0; i < doors.size(); i++)
    {
        if (doors[i].getRow() == newRow && doors[i].getCol() == newCol && !doors[i].getIsOpen())
        {
            cout << "\nYou can't walk through closed doors!\n";
            return false;
        }
    }

    for (int i = 0; i < guards.size(); i++)
    {
        if (newRow == guards[i].getRow() && newCol == guards[i].getCol())
        {
            playerRow = newRow;
            playerCol = newCol;

            printBoard();
            cout << "\nYou were caught!\n";
            gameOver = true;
            return true;
        }
    }

    if (board[newRow][newCol] == '$')
    {
        playerRow = newRow;
        playerCol = newCol;

        printBoard();
        cout << "\nYou win!\n";
        gameOver = true;
        return true;
    }

    playerRow = newRow;
    playerCol = newCol;

    triggerSwitches(playerRow, playerCol);

    return true;
}

void Game::moveGuards()
{
    for (int i = 0; i < guards.size(); i++)
    {
        int row = guards[i].getRow();
        int col = guards[i].getCol();

        int newRow = row;
        int newCol = col;

        char dir = guards[i].getDirection();

        if (dir == '^')
            newRow--;
        else if (dir == 'v')
            newRow++;
        else if (dir == '<')
            newCol--;
        else if (dir == '>')
            newCol++;

        if (newRow == playerRow && newCol == playerCol)
        {
            guards[i].setPosition(newRow, newCol);
            printBoard();
            cout << "\nYou were caught!\n";
            gameOver = true;
            return;
        }

        if (positionOccupied(newRow, newCol))
        {
            if (guards[i].getIsPatrolling())
            {
                guards[i].turnClockwise();
            }
            else
            {
                guards[i].reverseDirection();
            }

            dir = guards[i].getDirection();
            newRow = row;
            newCol = col;

            if (dir == '^')
                newRow--;
            else if (dir == 'v')
                newRow++;
            else if (dir == '<')
                newCol--;
            else if (dir == '>')
                newCol++;

            if (positionOccupied(newRow, newCol))
            {
                newRow = row;
                newCol = col;
            }
        }

        if (newRow == playerRow && newCol == playerCol)
        {
            guards[i].setPosition(newRow, newCol);
            printBoard();
            cout << "\nYou were caught!\n";
            gameOver = true;
            return;
        }

        guards[i].setPosition(newRow, newCol);
        triggerSwitches(newRow, newCol);
    }
}

bool Game::playerSeen()
{
    for (int i = 0; i < guards.size(); i++)
    {
        int row = guards[i].getRow();
        int col = guards[i].getCol();

        int dRow = 0;
        int dCol = 0;
        char dir = guards[i].getDirection();

        if (dir == '^')
            dRow = -1;
        else if (dir == 'v')
            dRow = 1;
        else if (dir == '<')
            dCol = -1;
        else if (dir == '>')
            dCol = 1;

        if (dRow == 0 && dCol == 0)
        {
            continue;
        }

        int currentRow = row + dRow;
        int currentCol = col + dCol;

        while (true)
        {
            if (currentRow < 0 || currentRow >= (int)board.size() ||
                currentCol < 0 || currentCol >= (int)board[currentRow].size())
            {
                break;
            }
            if (currentRow == playerRow && currentCol == playerCol)
            {
                return true;
            }
            if (board[currentRow][currentCol] == '#')
            {
                break;
            }

            if (board[currentRow][currentCol] == '$')
            {
                break;
            }

            bool doorBlocked = false;
            for (int k = 0; k < doors.size(); k++)
            {
                if (doors[k].getRow() == currentRow && doors[k].getCol() == currentCol && !doors[k].getIsOpen())
                {
                    doorBlocked = true;
                    break;
                }
            }
            if (doorBlocked)
            {
                break;
            }

            bool blocked = false;

            for (int j = 0; j < guards.size(); j++)
            {
                if (j != i && guards[j].getRow() == currentRow && guards[j].getCol() == currentCol)
                {
                    blocked = true;
                    break;
                }
            }

            if (blocked)
            {
                break;
            }
            currentRow += dRow;
            currentCol += dCol;
        }
    }

    return false;
}

void Game::handleInspect()
{
    int row, col;
    if (readTile(row, col))
    {
        inspectTile(row, col);
    }
}

void Game::triggerSwitches(int row, int col)
{
    for (int i = 0; i < switches.size(); i++)
    {
        if (switches[i].getRow() == row && switches[i].getCol() == col)
        {
            int groupId = switches[i].getGroupId();
            cout << "\nSwitch group " << groupId << " toggled!\n";

            for (int j = 0; j < doors.size(); j++)
            {
                if (doors[j].getGroupId() == groupId)
                {
                    doors[j].toggle();
                }
            }
        }
    }
}

void Game::createNewLevel()
{
    cout << "\nCreate a new level\n";

    cout << "Enter level name: ";
    string levelName;
    getline(cin, levelName);
    levelName = trim(levelName);

    while (levelName.empty())
    {
        cout << "Level name cannot be empty.\n";
        cout << "Enter level name: ";
        getline(cin, levelName);
        levelName = trim(levelName);
    }

    cout << "Enter number of rows: ";
    string rowText;
    getline(cin, rowText);
    rowText = trim(rowText);

    while (!isNumber(rowText) || stoi(rowText) <= 0)
    {
        cout << "Please enter a positive integer for rows.\n";
        cout << "Enter number of rows: ";
        getline(cin, rowText);
        rowText = trim(rowText);
    }

    cout << "Enter number of columns: ";
    string colText;
    getline(cin, colText);
    colText = trim(colText);

    while (!isNumber(colText) || stoi(colText) <= 0)
    {
        cout << "Please enter a positive integer for columns.\n";
        cout << "Enter number of columns: ";
        getline(cin, colText);
        colText = trim(colText);
    }

    editLevel(levelName, stoi(rowText), stoi(colText));
}

void Game::editLevel(const string &levelName, int rows, int cols)
{
    board.assign(rows, string(cols, ' '));
    guards.clear();
    doors.clear();
    switches.clear();
    playerRow = -1;
    playerCol = -1;
    currentLevelName = levelName;

    runEditor();
}

void Game::editExistingLevel()
{
    cout << "\nEdit an existing level\n";
    cout << "Enter the name of the level to edit (with or without .lvl): ";
    string name;
    getline(cin, name);
    name = trim(name);

    if (name.empty())
    {
        cout << "\nInvalid level name.\n";
        return;
    }

    if (!loadLevelFromFile(name))
    {
        return;
    }

    runEditor();
}

void Game::runEditor()
{
    while (true)
    {
        printEditorBoard();
        cout << "\nObject list:";
        cout << "\n 1) Wall";
        cout << "\n 2) Empty (clears/deletes a tile)";
        cout << "\n 3) Player";
        cout << "\n 4) Goal ($ - reach it to win)";
        cout << "\n 5) Guard";
        cout << "\n 6) Door";
        cout << "\n 7) Switch";
        cout << "\nI) Inspect a tile";
        cout << "\nR) Resize the level";
        cout << "\nM) Rename the level";
        cout << "\nS) Save and quit";
        cout << "\nQ) Quit without saving";
        cout << "\nChoose an object number: ";

        string line;
        getline(cin, line);
        if (!cin)
        {
            return;
        }

        line = trim(line);
        string upperLine = toUpper(line);

        if (upperLine == "S")
        {
            if (saveLevelToFile(currentLevelName))
            {
                addToLevelNames(currentLevelName);
                cout << "\nLevel saved successfully.\n";
            }
            return;
        }

        if (upperLine == "Q")
        {
            cout << "\nLevel discarded.\n";
            return;
        }

        if (upperLine == "I")
        {
            int row, col;
            if (readTile(row, col))
            {
                inspectTile(row, col);
            }
            continue;
        }

        if (upperLine == "R")
        {
            resizeLevel();
            continue;
        }

        if (upperLine == "M")
        {
            renameLevel();
            continue;
        }

        if (!isNumber(line))
        {
            cout << "\nPlease choose a valid object number.\n";
            continue;
        }

        int choice = stoi(line);
        if (choice < 1 || choice > 7)
        {
            cout << "\nPlease choose a valid object number.\n";
            continue;
        }

        if (choice == 1)
        {
            handleWallPlacement();
            continue;
        }

        int row, col;
        if (!readTile(row, col))
        {
            continue;
        }

        if (choice == 2)
        {
            placeEmpty(row, col);
        }
        else if (choice == 3)
        {
            placePlayer(row, col);
        }
        else if (choice == 4)
        {
            placeGoal(row, col);
        }
        else if (choice == 5)
        {
            placeGuard(row, col);
        }
        else if (choice == 6)
        {
            placeDoor(row, col);
        }
        else if (choice == 7)
        {
            placeSwitch(row, col);
        }
    }
}

bool Game::readTile(int &row, int &col)
{
    cout << "Enter row (0-indexed): ";
    string rowText;
    getline(cin, rowText);
    rowText = trim(rowText);

    cout << "Enter column (0-indexed): ";
    string colText;
    getline(cin, colText);
    colText = trim(colText);

    if (!isNumber(rowText) || !isNumber(colText))
    {
        cout << "\nPlease enter valid numbers.\n";
        return false;
    }

    row = stoi(rowText);
    col = stoi(colText);

    if (!isValidTile(row, col))
    {
        cout << "\nThat tile is out of bounds.\n";
        return false;
    }

    return true;
}
/*
Has 2 modes of walls. mode 1 is single wall placement, mode 2 is filling a 
box/range of walls. If the box is not a straight line, the user can choose 
to fill it solid or just outline it. This isn't part of the original acceptance criteria
its just a little something extra I added for ease of use when creating levels.
*/
void Game::handleWallPlacement()
{
    cout << "Wall mode (1=single wall, 2=fill a box/range): ";
    string mode;
    getline(cin, mode);
    mode = trim(mode);

    while (mode != "1" && mode != "2")
    {
        cout << "Please enter 1 or 2.\n";
        cout << "Wall mode (1=single wall, 2=fill a box/range): ";
        getline(cin, mode);
        mode = trim(mode);
    }

    if (mode == "1")
    {
        int row, col;
        if (readTile(row, col))
        {
            placeWall(row, col);
        }
        return;
    }

    cout << "Enter the TOP-LEFT corner of the box.\n";
    int r1, c1;
    if (!readTile(r1, c1))
    {
        return;
    }
    cout << "Enter the BOTTOM-RIGHT corner of the box.\n";
    int r2, c2;
    if (!readTile(r2, c2))
    {
        return;
    }
    int topRow = r1 < r2 ? r1 : r2;
    int bottomRow = r1 < r2 ? r2 : r1;
    int leftCol = c1 < c2 ? c1 : c2;
    int rightCol = c1 < c2 ? c2 : c1;

    bool outlineOnly = false;
    bool straightLine = (topRow == bottomRow) || (leftCol == rightCol);
    if (!straightLine)
    {
        cout << "Fill style (1=solid box, 2=outline only): ";
        string fill;
        getline(cin, fill);
        fill = trim(fill);

        while (fill != "1" && fill != "2")
        {
            cout << "Please enter 1 or 2.\n";
            cout << "Fill style (1=solid box, 2=outline only): ";
            getline(cin, fill);
            fill = trim(fill);
        }

        outlineOnly = (fill == "2");
    }
    int count = 0;
    for (int r = topRow; r <= bottomRow; r++)
    {
        for (int c = leftCol; c <= rightCol; c++)
        {
            bool edge = (r == topRow || r == bottomRow || c == leftCol || c == rightCol);
            if (outlineOnly && !edge)
            {
                continue;
            }
            clearObjectsAt(r, c);
            board[r][c] = '#';
            count++;
        }
    }

    cout << "\nPlaced " << count << " wall" << (count == 1 ? "" : "s") << ".\n";
}
/*
This is here in case the user wants to resize the level after initially creating it. 
It will remove any objects that are outside the new size.
*/
void Game::resizeLevel()
{
    int oldRows = (int)board.size();
    int oldCols = board.empty() ? 0 : (int)board[0].size();
    cout << "\nCurrent size: " << oldRows << " rows x " << oldCols << " cols\n";
    cout << "Objects outside the new size will be removed.\n";

    cout << "Enter new number of rows: ";
    string rowText;
    getline(cin, rowText);
    rowText = trim(rowText);
    while (!isNumber(rowText) || stoi(rowText) <= 0)
    {
        cout << "Please enter a positive integer for rows.\n";
        cout << "Enter new number of rows: ";
        getline(cin, rowText);
        rowText = trim(rowText);
    }

    cout << "Enter new number of columns: ";
    string colText;
    getline(cin, colText);
    colText = trim(colText);
    while (!isNumber(colText) || stoi(colText) <= 0)
    {
        cout << "Please enter a positive integer for columns.\n";
        cout << "Enter new number of columns: ";
        getline(cin, colText);
        colText = trim(colText);
    }
    int newRows = stoi(rowText);
    int newCols = stoi(colText);

    vector<string> newBoard(newRows, string(newCols, ' '));
    for (int r = 0; r < newRows && r < oldRows; r++)
    {
        for (int c = 0; c < newCols && c < oldCols; c++)
        {
            newBoard[r][c] = board[r][c];
        }
    }
    board = newBoard;

    for (int i = (int)guards.size() - 1; i >= 0; i--)
    {
        if (!isValidTile(guards[i].getRow(), guards[i].getCol()))
        {
            guards.erase(guards.begin() + i);
        }
    }
    for (int i = (int)doors.size() - 1; i >= 0; i--)
    {
        if (!isValidTile(doors[i].getRow(), doors[i].getCol()))
        {
            doors.erase(doors.begin() + i);
        }
    }
    for (int i = (int)switches.size() - 1; i >= 0; i--)
    {
        if (!isValidTile(switches[i].getRow(), switches[i].getCol()))
        {
            switches.erase(switches.begin() + i);
        }
    }
    if (playerRow >= 0 && !isValidTile(playerRow, playerCol))
    {
        playerRow = -1;
        playerCol = -1;
    }

    cout << "\nLevel resized to " << newRows << " x " << newCols << ".\n";
}
void Game::renameLevel()
{
    cout << "\nCurrent level name: " << currentLevelName << "\n";
    cout << "Enter new level name: ";
    string newName;
    getline(cin, newName);
    newName = stripLvlExtension(newName);

    while (newName.empty())
    {
        cout << "Level name cannot be empty.\n";
        cout << "Enter new level name: ";
        getline(cin, newName);
        newName = stripLvlExtension(newName);
    }

    string oldName = currentLevelName;
    if (toUpper(oldName) == toUpper(newName))
    {
        currentLevelName = newName;
        cout << "\nName unchanged.\n";
        return;
    }

    string oldPath = getLevelFilePath(oldName);
    if (filesystem::exists(oldPath))
    {
        filesystem::create_directories(getLevelDirectory());
        filesystem::rename(oldPath, getLevelFilePath(newName));
    }

    for (int i = 0; i < (int)levelNames.size(); i++)
    {
        if (toUpper(levelNames[i]) == toUpper(oldName))
        {
            levelNames[i] = newName;
            break;
        }
    }

    currentLevelName = newName;
    cout << "\nLevel renamed to '" << newName << "'.\n";
}

void Game::addToLevelNames(const string &name)
{
    string cleanedName = trim(name);
    if (cleanedName.empty())
    {
        return;
    }

    string upperName = toUpper(cleanedName);
    for (int i = 0; i < levelNames.size(); i++)
    {
        if (toUpper(levelNames[i]) == upperName)
        {
            return;
        }
    }

    levelNames.push_back(cleanedName);
}
bool Game::saveLevelToFile(const string &levelName) const
{
    string name = stripLvlExtension(levelName);
    if (name.empty())
    {
        cout << "\nInvalid level name.\n";
        return false;
    }

    filesystem::create_directories(getLevelDirectory());
    ofstream output(getLevelFilePath(name));
    if (!output.is_open())
    {
        cout << "\nUnable to save the level file.\n";
        return false;
    }

    output << board.size() << " " << (board.empty() ? 0 : board[0].size()) << "\n";

    if (playerRow >= 0 && playerCol >= 0)
    {
        output << "player " << playerRow << " " << playerCol << "\n";
    }

    for (int row = 0; row < (int)board.size(); row++)
    {
        for (int col = 0; col < (int)board[row].size(); col++)
        {
            if (board[row][col] == '#')
            {
                output << "wall " << row << " " << col << "\n";
            }
            else if (board[row][col] == '$')
            {
                output << "goal " << row << " " << col << "\n";
            }
        }
    }
    for (int i = 0; i < guards.size(); i++)
    {
        output << "guard " << guards[i].getRow() << " " << guards[i].getCol() << " "
               << (guards[i].getIsPatrolling() ? "Patrolling" : "Normal") << " "
               << guards[i].getDirection() << "\n";
    }

    for (int i = 0; i < doors.size(); i++)
    {
        output << "door " << doors[i].getRow() << " " << doors[i].getCol() << " "
               << doors[i].getGroupId() << "\n";
    }

    for (int i = 0; i < switches.size(); i++)
    {
        output << "switch " << switches[i].getRow() << " " << switches[i].getCol() << " "
               << switches[i].getGroupId() << "\n";
    }

    return true;
}

void Game::clearObjectsAt(int row, int col)
{
    for (int i = guards.size() - 1; i >= 0; i--)
    {
        if (guards[i].getRow() == row && guards[i].getCol() == col)
        {
            guards.erase(guards.begin() + i);
        }
    }
    for (int i = doors.size() - 1; i >= 0; i--)
    {
        if (doors[i].getRow() == row && doors[i].getCol() == col)
        {
            doors.erase(doors.begin() + i);
        }
    }

    for (int i = switches.size() - 1; i >= 0; i--)
    {
        if (switches[i].getRow() == row && switches[i].getCol() == col)
        {
            switches.erase(switches.begin() + i);
        }
    }
    if (playerRow == row && playerCol == col)
    {
        playerRow = -1;
        playerCol = -1;
    }
}

void Game::placeWall(int row, int col)
{
    clearObjectsAt(row, col);
    board[row][col] = '#';
    cout << "\nWall placed.\n";
}
// handles deleting objects and placing empty space
void Game::placeEmpty(int row, int col)
{
    clearObjectsAt(row, col);
    board[row][col] = ' ';
    cout << "\nTile cleared.\n";
}
void Game::placeGoal(int row, int col)
{
    clearObjectsAt(row, col);
    board[row][col] = '$';
    cout << "\nGoal ($) placed.\n";
}
// handles adding player to level
void Game::placePlayer(int row, int col)
{
    clearObjectsAt(row, col);
    board[row][col] = ' ';
    playerRow = row;
    playerCol = col;
    cout << "\nPlayer placed.\n";
}
// two diff types of gaurds
void Game::placeGuard(int row, int col)
{
    cout << "Movement type (1=Normal, 2=Patrolling): ";
    string movement;
    getline(cin, movement);
    movement = trim(movement);

    while (movement != "1" && movement != "2")
    {
        cout << "Please enter 1 or 2.\n";
        cout << "Movement type (1=Normal, 2=Patrolling): ";
        getline(cin, movement);
        movement = trim(movement);
    }

    cout << "Direction (W/A/S/D): ";
    string direction;
    getline(cin, direction);
    direction = trim(direction);
    direction = toUpper(direction);

    while (direction != "W" && direction != "A" && direction != "S" && direction != "D")
    {
        cout << "Please enter W, A, S, or D.\n";
        cout << "Direction (W/A/S/D): ";
        getline(cin, direction);
        direction = trim(direction);
        direction = toUpper(direction);
    }
    char dirChar = '^';
    if (direction == "W")
        dirChar = '^';
    else if (direction == "A")
        dirChar = '<';
    else if (direction == "S")
        dirChar = 'v';
    else if (direction == "D")
        dirChar = '>';

    clearObjectsAt(row, col);
    board[row][col] = ' ';
    guards.push_back(Guard(row, col, dirChar, movement == "2"));
    cout << "\nGuard placed.\n";
}
// doors need ID's to their corresponding switches
void Game::placeDoor(int row, int col)
{
    cout << "Enter door group id: ";
    string groupText;
    getline(cin, groupText);
    groupText = trim(groupText);

    while (!isNumber(groupText) || stoi(groupText) < 0)
    {
        cout << "Please enter a non-negative integer.\n";
        cout << "Enter door group id: ";
        getline(cin, groupText);
        groupText = trim(groupText);
    }

    clearObjectsAt(row, col);
    board[row][col] = ' ';
    doors.push_back(Door(row, col, stoi(groupText)));
    cout << "\nDoor placed.\n";
}
// switches need to have ID's to their corresponding doors, so they can be toggled
void Game::placeSwitch(int row, int col)
{
    cout << "Enter switch group id: ";
    string groupText;
    getline(cin, groupText);
    groupText = trim(groupText);

    while (!isNumber(groupText) || stoi(groupText) < 0)
    {
        cout << "Please enter a non-negative integer.\n";
        cout << "Enter switch group id: ";
        getline(cin, groupText);
        groupText = trim(groupText);
    }
    clearObjectsAt(row, col);
    board[row][col] = ' ';
    switches.push_back(Switch(row, col, stoi(groupText)));
    cout << "\nSwitch placed.\n";
}
// shared between editor and game mode
void Game::inspectTile(int row, int col) const
{
    cout << "\n--- Inspection ---\n";

    if (row == playerRow && col == playerCol)
    {
        cout << "Tile contains: Player (@)\n";
        return;
    }

    for (int i = 0; i < guards.size(); i++)
    {
        if (guards[i].getRow() == row && guards[i].getCol() == col)
        {
            cout << "Tile contains: Guard (";
            if (guards[i].getIsPatrolling())
            {
                cout << "Patrolling, ";
            }
            else
            {
                cout << "Normal, ";
            }
            cout << "Direction: " << guards[i].getDirection() << ")\n";
            return;
        }
    }

    for (int i = 0; i < doors.size(); i++)
    {
        if (doors[i].getRow() == row && doors[i].getCol() == col)
        {
            cout << "Tile contains: Door (Group " << doors[i].getGroupId() << ")\n";
            return;
        }
    }

    for (int i = 0; i < switches.size(); i++)
    {
        if (switches[i].getRow() == row && switches[i].getCol() == col)
        {
            cout << "Tile contains: Switch (Group " << switches[i].getGroupId() << ")\n";
            return;
        }
    }
    char tile = board[row][col];
    if (tile == '#')
    {
        cout << "Tile contains: Wall (#)\n";
    }
    else if (tile == '$')
    {
        cout << "Tile contains: Goal ($)\n";
    }
    else if (tile == ' ')
    {
        cout << "Tile contains: Empty space\n";
    }
    else
    {
        cout << "Tile contains: " << tile << "\n";
    }
}
bool Game::isValidTile(int row, int col) const
{
    return row >= 0 && row < (int)board.size() && col >= 0 && col < (int)board[row].size();
}
// returns the directory where level files are stored
string Game::getLevelDirectory() const
{
    return "levels";
}
string Game::getLevelFilePath(const string &levelName) const
{
    return getLevelDirectory() + "/" + levelName + ".lvl";
}

string Game::stripLvlExtension(const string &name) const
{
    string cleaned = trim(name);
    if (cleaned.size() >= 4 && toUpper(cleaned.substr(cleaned.size() - 4)) == ".LVL")
    {
        cleaned = cleaned.substr(0, cleaned.size() - 4);
    }
    return cleaned;
}
