#include "Switch.h"

Switch::Switch(int r, int c, int id)
{
    row = r;
    col = c;
    groupId = id;
}

int Switch::getRow() const
{
    return row;
}

int Switch::getCol() const
{
    return col;
}

int Switch::getGroupId() const
{
    return groupId;
}

void Switch::setPosition(int r, int c)
{
    row = r;
    col = c;
}
