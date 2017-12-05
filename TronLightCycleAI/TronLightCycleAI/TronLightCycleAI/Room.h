#pragma once
#include "CommonDefs.h"
#include "Node.h"
#include "MapNode.h"

using namespace std;

static int roomID = 0;

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
    int GetID() { return id; }
    bool HasExits() { return m_exitIndex.size() > 0; }

private:

    vector<int> m_exitIndex;
    vector<Node> m_nodeList;
    int id;
};

