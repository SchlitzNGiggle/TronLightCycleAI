#pragma once
#include "CommonDefs.h"
#include "Node.h"
#include "MapNode.h"

using namespace std;

class Room
{
public:
    Room();
    ~Room();

    void AddNode(int row, int column);
    bool Contains(int row, int column);
    void BuildRoom(vector<MapNode>& mapNodeList, int rows, int columns, fstream& worldMapLog);
    void PrintRoom(fstream& worldMapLog);
    int GetSize() { return static_cast<int>(m_nodeList.size()); }

private:

    vector<Node> m_nodeList;
};

