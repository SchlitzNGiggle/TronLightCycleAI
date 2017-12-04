#pragma once
#include "Node.h"
class MapNode :
    public Node
{
public:
    MapNode();
    ~MapNode();

    void Initialize(const int row, const int column, char tileType);
    char GetTileType();
    char GetStructure();
    void SetRoom();
    void SetHallway();
    void SetIntersection();

private:
    char m_tileType;
    structureType m_myStructureType;
};

