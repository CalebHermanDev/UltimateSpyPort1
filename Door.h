#pragma once

class Door
{
private:
    int row;
    int col;
    int groupId;
    bool isOpen;

public:
    Door(int r, int c, int id);

    int getRow() const;
    int getCol() const;
    int getGroupId() const;
    bool getIsOpen() const;

    void setPosition(int r, int c);
    void toggle();
};
