#pragma once
#include "CommonDefs.h"
#include "Node.h"
#include "MapNode.h"

using namespace std;

static int hallwayID = 0;

class Hallway
{
public:
    Hallway();
    ~Hallway();

    void AddNode(int row, int column);
    bool Contains(int row, int column);
    void BuildHallway(vector<MapNode>& mapNodeList, int rows, int columns, fstream& worldMapLog);
    void PrintHallway(fstream& worldMapLog);
    int GetID() { return id; }

private:
    vector<Node> m_hallwayNodes;
    int id;
};

