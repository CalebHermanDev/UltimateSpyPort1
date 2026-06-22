#include "Game.h"
#include <iostream>
#include <cctype>

using namespace std;

void Game::run()
{
    showMenu();

    gameOver = false;

    while (!gameOver)
    {
        printBoard();
        cout << "\nMove (WASD) or Q to quit: ";

        char input;
        cin >> input;
        input = toupper(input);

        if (input == 'Q')
            break;

        movePlayer(input);
        if (!gameOver)
        {
            moveGuards();
        }

        if (!gameOver && playerSeen())
        {
            printBoard();

            cout << "\nYou were spotted!\n";

            gameOver = true;
        }
    }
}

void Game::showMenu()
{
    cout << "Welcome to Ultra-Spy!\n";
    cout << "Select a level:\n";
    cout << "1) A New Enemy\n";
    cout << "2) The Truth of the Weapon\n\n";
    int choice;
    cin >> choice;

    loadLevel(choice);
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

        guards.clear();

        guards.push_back(Guard(1, 8, 'v'));
        guards.push_back(Guard(3, 5, '>'));
        guards.push_back(Guard(5, 3, '^'));
    }
}

void Game::findPlayer()
{
    if (board.size() == 6)
    {
        playerRow = 4;
        playerCol = 1;
    }
    else
    {
        playerRow = 1;
        playerCol = 1;
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
void Game::movePlayer(char input)
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
        return;

    for (int i = 0; i < guards.size(); i++)
    {
        if (newRow == guards[i].getRow() &&
            newCol == guards[i].getCol())
        {
            cout << "\nYou were caught!\n";
            gameOver = true;
            return;
        }
    }

    if (board[newRow][newCol] == '$')
    {
        playerRow = newRow;
        playerCol = newCol;

        printBoard();
        cout << "\nYou win!\n";
        gameOver = true;
        return;
    }
    playerRow = newRow;
    playerCol = newCol;
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