#pragma once
#include "CommonDefs.h"

class Node
{
public:
    Node();
    ~Node();

    void SetCoordinates(int row, int column);
    inline bool HasSameCoordinates(int row, int column) { return (m_row == row) && (m_column == column); }
    int GetRow() { return m_row; }
    int GetColumn() { return m_column; }

protected:
    int m_row;
    int m_column;
};

