#include "stdafx.h"
#include "MapNode.h"


MapNode::MapNode()
{
    m_tileType = FLOOR;
    m_myStructureType = NONE;
}


MapNode::~MapNode()
{
}

void MapNode::Initialize(const int row, const int column, char tileType)
{
    m_row = row;
    m_column = column;
    m_tileType = tileType;
    m_myStructureType = NONE;
}

char MapNode::GetTileType()
{
    return m_tileType;
}

char MapNode::GetStructure()
{
    switch (m_myStructureType)
    {

    case structureType::RoomTile:
    {
        return ROOM;
    }
    break;

    case structureType::Hallway:
    {
        return HALLWAY;
    }
    break;

    case structureType::Intersection:
    {
        return INTERSECTION;
    }
    break;

    }
    return WALL;
}

void MapNode::SetRoom()
{
    m_myStructureType = structureType::RoomTile;
}

void MapNode::SetHallway()
{
    m_myStructureType = structureType::Hallway;
}

void MapNode::SetIntersection()
{
    m_myStructureType = structureType::Intersection;
}
