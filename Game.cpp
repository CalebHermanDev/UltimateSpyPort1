#include "Game.h"
#include <iostream>
#include <cctype>

using namespace std;

Game::Game()
{
    gameOver = false;
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

        loadLevel(levelChoice);
        gameOver = false;

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
        }

        cout << "\nInvalid selection. Please try again.\n";
    }
}

bool Game::isNumber(const string &text)
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

string Game::toUpper(const string &text)
{
    string result = text;

    for (int i = 0; i < result.size(); i++)
    {
        result[i] = toupper(result[i]);
    }

    return result;
}

string Game::trim(const string &text)
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
        // Patrolling Rectangle - guard walks in a rectangle with clockwise turns
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

        guards.push_back(Guard(1, 5, '>', true)); // true = patrolling
    }
    else if (levelNumber == 4)
    {
        // Complex Patrol - guard follows a more complex path
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

        // Patrolling guard at a starting position
        guards.push_back(Guard(2, 11, '<', true));
    }
    else if (levelNumber == 5)
    {
        // Multi-Group Doors/Switches
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

        // Red group (group 0): switch at (5,5), doors at (1,7) and (2,7) and (3,7)
        switches.push_back(Switch(5, 5, 0));
        doors.push_back(Door(1, 7, 0));
        doors.push_back(Door(2, 7, 0));
        doors.push_back(Door(3, 7, 0));

        // Blue group (group 1): switch at (5,14), doors at (1,12), (2,12), (3,12)
        switches.push_back(Switch(5, 14, 1));
        doors.push_back(Door(1, 12, 1));
        doors.push_back(Door(2, 12, 1));
        doors.push_back(Door(3, 12, 1));

        // Guard moving around
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

        // Red group (0)
        switches.push_back(Switch(5, 5, 0));
        doors.push_back(Door(1, 6, 0));

        // Blue group (1)
        switches.push_back(Switch(6, 18, 1));
        doors.push_back(Door(3, 11, 1));
        doors.push_back(Door(4, 11, 1));
        doors.push_back(Door(5, 11, 1));

        // Green group (2)
        switches.push_back(Switch(6, 8, 2));
        doors.push_back(Door(4, 19, 2));
        doors.push_back(Door(5, 19, 2));

        // Linear guard moving up and down
        guards.push_back(Guard(1, 5, 'v'));

        // Clockwise patrolling guard on right
        guards.push_back(Guard(1, 13, '>', true));
    }
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
        tempBoard[guards[i].getRow()][guards[i].getCol()] =
            guards[i].getDirection();
    }

    tempBoard[playerRow][playerCol] = '@';
    cout << endl;

    for (string row : tempBoard)
    {
        cout << row << endl;
    }
}
bool Game::positionOccupied(int row, int col)
{
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
        if (guards[i].getRow() == row &&
            guards[i].getCol() == col)
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
        if (newRow == guards[i].getRow() &&
            newCol == guards[i].getCol())
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

        int currentRow = row + dRow;
        int currentCol = col + dCol;

        while (true)
        {
            if (currentRow == playerRow &&
                currentCol == playerCol)
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
                if (j != i &&
                    guards[j].getRow() == currentRow &&
                    guards[j].getCol() == currentCol)
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
    cout << "\nEnter row (0-indexed): ";
    string rowStr;
    getline(cin, rowStr);
    rowStr = trim(rowStr);

    cout << "Enter column (0-indexed): ";
    string colStr;
    getline(cin, colStr);
    colStr = trim(colStr);

    if (!isNumber(rowStr) || !isNumber(colStr))
    {
        cout << "\nInvalid input. Please enter valid numbers.\n";
        return;
    }

    int row = stoi(rowStr);
    int col = stoi(colStr);

    if (row < 0 || row >= (int)board.size() || col < 0 || col >= (int)board[row].size())
    {
        cout << "\nTile is out of bounds.\n";
        return;
    }

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
                cout << "Patrolling, ";
            else
                cout << "Linear, ";
            cout << "Direction: " << guards[i].getDirection() << ")\n";
            return;
        }
    }

    for (int i = 0; i < doors.size(); i++)
    {
        if (doors[i].getRow() == row && doors[i].getCol() == col)
        {
            cout << "Tile contains: Door (Group " << doors[i].getGroupId() << ", ";
            cout << (doors[i].getIsOpen() ? "Open" : "Closed") << ")\n";
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
