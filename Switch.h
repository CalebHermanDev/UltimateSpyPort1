#pragma once

class Switch
{
private:
    int row;
    int col;
    int groupId;

public:
    Switch(int r, int c, int id);

    int getRow() const;
    int getCol() const;
    int getGroupId() const;

    void setPosition(int r, int c);
};
