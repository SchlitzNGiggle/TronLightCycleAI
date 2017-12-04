#pragma once
#include "CommonDefs.h"
#include "Node.h"
#include "MapNode.h"

using namespace std;

class Hallway
{
public:
    Hallway();
    ~Hallway();

    void AddNode(int row, int column);
    bool Contains(int row, int column);
    void BuildHallway(vector<MapNode>& mapNodeList, int rows, int columns, fstream& worldMapLog);
    void PrintHallway(fstream& worldMapLog);

private:
    vector<Node> m_hallwayNodes;;
};

