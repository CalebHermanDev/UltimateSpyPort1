#pragma once

class Guard
{
private:
    int row;
    int col;
    char direction;
    bool isPatrolling;

public:
    Guard(int r, int c, char d, bool patrolling = false);

    int getRow() const;
    int getCol() const;
    char getDirection() const;
    bool getIsPatrolling() const;

    void setPosition(int r, int c);
    void reverseDirection();
    void turnClockwise();
};