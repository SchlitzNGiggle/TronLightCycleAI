#include "stdafx.h"
#include "PathNode.h"


PathNode::PathNode()
{
    m_cost = -1;
    m_previousRow = -1;
    m_previousColumn = 1;
    m_manhattanDistance = -1;
    m_previousNode = nullptr;
}


PathNode::~PathNode()
{
}
