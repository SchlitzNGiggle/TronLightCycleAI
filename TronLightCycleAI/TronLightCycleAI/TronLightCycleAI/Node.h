#pragma once
#include "CommonDefs.h"

class Node
{
public:
    Node();
    ~Node();

    void SetCoordinates(int row, int column);
    bool HasSameCoordinates(int row, int column);
    int GetRow() const { return m_row; }
    int GetColumn() const { return m_column; }

protected:
    int m_row;
    int m_column;
};

