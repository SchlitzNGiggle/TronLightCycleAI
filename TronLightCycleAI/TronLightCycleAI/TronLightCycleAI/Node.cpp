#include "stdafx.h"
#include "Node.h"

Node::Node()
{
    m_row = -1;
    m_column = -1;
}


Node::~Node()
{
}

void Node::SetCoordinates(int row, int column)
{
    m_row = row;
    m_column = column;
}

bool Node::HasSameCoordinates(int row, int column)
{
    return (m_row == row) && (m_column == column);
}