#pragma once
#include "Node.h"
class PathNode :
    public Node
{
public:
    PathNode();
    ~PathNode();

    void SetCost(int cost) { m_cost = cost; }
    void SetPrevious(int row, int column) { m_previousRow = row; m_previousColumn = column; }
    void SetPreviousNode(PathNode* previous) { m_previousNode = previous; }
    void SetManhattanDistance(int distance) { m_manhattanDistance = distance; }
    int GetDistance() const { return m_manhattanDistance; }
    int GetPreviousRow() { return m_previousRow; }
    int GetPreviousColumn() { return m_previousColumn; }
    PathNode* GetPreviousNode() { return m_previousNode; }
    
private:
    int m_cost;

    int m_previousRow;
    int m_previousColumn;
    PathNode* m_previousNode;
    int m_manhattanDistance;
};

