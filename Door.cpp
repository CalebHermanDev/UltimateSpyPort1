#include "Door.h"

Door::Door(int r, int c, int id)
{
    row = r;
    col = c;
    groupId = id;
    isOpen = false;
}

int Door::getRow() const
{
    return row;
}

int Door::getCol() const
{
    return col;
}

int Door::getGroupId() const
{
    return groupId;
}

bool Door::getIsOpen() const
{
    return isOpen;
}

void Door::setPosition(int r, int c)
{
    row = r;
    col = c;
}

void Door::toggle()
{
    isOpen = !isOpen;
}
