#include "Guard.h"

Guard::Guard(int r, int c, char d)
{
    row = r;
    col = c;
    direction = d;
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