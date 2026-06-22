#pragma once

class Guard
{
private:
    int row;
    int col;
    char direction;

public:
    Guard(int r, int c, char d);

    int getRow() const;
    int getCol() const;
    char getDirection() const;

    void setPosition(int r, int c);
    void reverseDirection();
};