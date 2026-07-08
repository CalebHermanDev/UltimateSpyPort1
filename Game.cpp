#include "Game.h"
#include <iostream>
#include <cctype>

using namespace std;

Game::Game()
{
    levelNames.push_back("A New Enemy");
    levelNames.push_back("The Truth of the Weapon");
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
            cout << "\nMove (WASD) or Q to quit: ";

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
                cout << "\nInvalid input. Please enter W, A, S, D, or Q.\n";
                continue;
            }

            char input = toupper(line[0]);

            if (input != 'W' && input != 'A' && input != 'S' &&
                input != 'D' && input != 'Q')
            {
                cout << "\nInvalid input. Please enter W, A, S, D, or Q.\n";
                continue;
            }

            if (input == 'Q')
            {
                quit = true;
                break;
            }

            bool moved = movePlayer(input);

            if (gameOver)
            {
                break;
            }

            if (!moved)
            {
                // Move was blocked (e.g. a wall), so the turn was not spent.
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
    else
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
}

void Game::printBoard()
{
    vector<string> tempBoard = board;

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

    if (board[newRow][newCol] == '#')
    {
        cout << "\nYou can't walk through walls!\n";
        return false;
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

        if (positionOccupied(newRow, newCol))
        {
            guards[i].reverseDirection();

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

            // If the opposite direction is blocked too,
            // stay where you are.
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
