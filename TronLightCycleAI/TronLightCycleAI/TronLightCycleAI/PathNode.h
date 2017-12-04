#pragma once
#include "Node.h"
class PathNode :
    public Node
{
public:
    PathNode();
    ~PathNode();

    void SetCost(int cost) { m_cost = cost; }

private:
    int m_cost;
};

