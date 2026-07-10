#include "Guard.h"

Guard::Guard(int r, int c, char d, bool patrolling)
{
    row = r;
    col = c;
    direction = d;
    isPatrolling = patrolling;
}

int Guard::getRow() const
{
    return row;
}

int Guard::getCol() const
{
    return col;
}

char Guard::getDirection() const
{
    return direction;
}

bool Guard::getIsPatrolling() const
{
    return isPatrolling;
}

void Guard::setPosition(int r, int c)
{
    row = r;
    col = c;
}

void Guard::reverseDirection()
{
    if (direction == '^')
        direction = 'v';
    else if (direction == 'v')
        direction = '^';
    else if (direction == '<')
        direction = '>';
    else if (direction == '>')
        direction = '<';
}

void Guard::turnClockwise()
{
    // Clockwise turn: ^ > v < ^
    if (direction == '^')
        direction = '>';
    else if (direction == '>')
        direction = 'v';
    else if (direction == 'v')
        direction = '<';
    else if (direction == '<')
        direction = '^';
}