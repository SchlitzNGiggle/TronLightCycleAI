#include "stdafx.h"
#include "WorldMap.h"

#include "Node.h"
#include "MapNode.h"

#define LOGFILE "WorldMap.txt"

WorldMap::WorldMap(int rows, int columns)
{
    m_rows = rows;
    m_columns = columns;
    m_nodeList.reserve(rows * columns);
    m_roomList.reserve(rows * columns);

    for (int i = 0; i < rows * columns; i++)
    {
        MapNode* newNode = new MapNode();
        m_nodeList.push_back(*newNode);
    }

    m_worldMapLog.open(LOGFILE, ios::trunc|ios::out);
    m_worldMapLog << "Rows: " << m_rows << " Columns: " << columns << endl;
}


WorldMap::~WorldMap()
{
    m_worldMapLog.close();
}

void WorldMap::TagRooms()
{
    //m_worldMapLog << "BuildingRooms" << endl;

    int validNeighborCount;
    bool topClear, bottomClear, leftClear, rightClear;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            if (m_nodeList[(i * m_rows) + j].GetTileType() == WALL)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER1)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER2)
            {
                continue;
            }

            validNeighborCount = 0;
            topClear = false;
            bottomClear = false;
            leftClear = false;
            rightClear = false;

            char currentTypeType = GetNorthTileType(i, j);
            if (currentTypeType == FLOOR)
            {
                validNeighborCount++;
                topClear = true;
            }

            currentTypeType = GetEastTileType(i, j);
            if (currentTypeType == FLOOR)
            {
                validNeighborCount++;
                rightClear = true;
            }

            currentTypeType = GetSouthTileType(i, j);
            if (currentTypeType == FLOOR)
            {
                validNeighborCount++;
                bottomClear = true;
            }

            currentTypeType = GetWestTileType(i, j);
            if (currentTypeType == FLOOR)
            {
                validNeighborCount++;
                leftClear = true;
            }

            if (validNeighborCount > 2)
            {
                m_nodeList[(i * m_rows) + j].SetRoom();
            }
            else if (topClear && bottomClear)
            {
                continue;
            }
            else if (leftClear && rightClear)
            {
                continue;
            }
            else if (validNeighborCount == 2)
            {
                if ((GetNorthEastTileType(i, j) == FLOOR)
                    || (GetNorthWestTileType(i, j) == FLOOR)
                    || (GetSouthEastTileType(i, j) == FLOOR)
                    || (GetSouthWestTileType(i, j) == FLOOR))
                {
                    m_nodeList[(i * m_rows) + j].SetRoom();
                }
            }
        }
    }
}

void WorldMap::TagHallway()
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {            
            if (m_nodeList[(i * m_rows) + j].GetTileType() == WALL)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER1)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER2)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetStructure() == ROOM)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == FLOOR)
            {
                m_nodeList[(i * m_rows) + j].SetHallway();
            }
        }
    }
}

void WorldMap::TagIntersections()
{
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            if (m_nodeList[(i * m_rows) + j].GetTileType() == WALL)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER1)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetTileType() == PLAYER2)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetStructure() == INTERSECTION)
            {
                continue;
            }

            if (m_nodeList[(i * m_rows) + j].GetStructure() == ROOM)
            {
                int adjacentHallwaysCount = 0;
                if (m_nodeList[(i * m_rows) + (j - 1)].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[(i * m_rows) + (j + 1)].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[((i + 1) * m_rows) + j].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[((i - 1) * m_rows) + j].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (adjacentHallwaysCount >= 3)
                {
                    m_nodeList[(i * m_rows) + j].SetIntersection();
                }
            }
        }
    }
}

void WorldMap::ProcessRooms()
{
    //m_worldMapLog << "Trying to build rooms" << endl;
    m_roomList.clear();

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int realRow = i * m_rows;
            //m_worldMapLog << "Checking: " << i << " " << j << endl;
            if ( (m_nodeList[realRow + j].GetStructure() == ROOM)
                && !IsTileInRoomList(i, j) )
            {
                //m_worldMapLog << "Size of room list: " << m_roomList.size() << endl;
                //m_worldMapLog << "Building a room: " << i << " " << j << endl;
                // build a new room
                Room* room = new Room();
                room->AddNode(i, j);
                room->BuildRoom(m_nodeList, m_rows, m_columns, m_worldMapLog);
                //room->PrintRoom(m_worldMapLog);
                m_roomList.push_back(*room);
            }
        }
    }

    m_worldMapLog << m_roomList.size() << " Rooms made." << endl;
    for (auto i = 0; i < m_roomList.size(); i++)
    {
        m_roomList[i].PrintRoom(m_worldMapLog);
    }
}

void WorldMap::ProcessHallways()
{
    m_hallwayList.clear();
    //m_worldMapLog << "Trying to build hallways" << endl;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int realRow = i * m_rows;
            if ((m_nodeList[realRow + j].GetStructure() == HALLWAY)
                && !IsTileInHallwayList(i, j))
            {
                //m_worldMapLog << "Building a Hallway: " << i << " " << j << endl;
                // build a new room
                Hallway* hallway = new Hallway();
                hallway->AddNode(i, j);
                hallway->BuildHallway(m_nodeList, m_rows, m_columns, m_worldMapLog);
                m_hallwayList.push_back(*hallway);
            }
        }
    }

    m_worldMapLog << m_hallwayList.size() << " Hallways built." << endl;
    //for (auto i = 0; i < m_hallwayList.size(); i++)
    //{
    //    m_hallwayList[i].PrintHallway(m_worldMapLog);
    //}
}

bool WorldMap::IsTileInRoomList(int row, int column)
{
    auto roomListSize = m_roomList.size();
    for (int i = 0; i < roomListSize; i++)
    {
        if (m_roomList[i].Contains(row, column))
        {
            return true;
        }
    }

    return false;
}

bool WorldMap::IsTileInHallwayList(int row, int column)
{
    auto hallwayListSize = m_hallwayList.size();
    if (hallwayListSize > 0)
    {
        for (auto i = 0; i < hallwayListSize; i++)
        {
            if (m_hallwayList[i].Contains(row, column))
            {
                return true;
            }
        }
    }

    return false;
}

char WorldMap::GetNorthEastTileType(int row, int column)
{
    if ((row > 0) && (column < m_columns - 1))
    {
        return m_nodeList[((row - 1) * m_rows) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetNorthWestTileType(int row, int column)
{
    if ((row > 0) && (column > 0))
    {
        return m_nodeList[((row - 1) * m_rows) + (column - 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthEastTileType(int row, int column)
{
    if ((row < m_rows - 1) && (column < m_columns - 1))
    {
        return m_nodeList[((row + 1) * m_rows) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthWestTileType(int row, int column)
{
    if ((row < m_rows - 1) && (column > 0))
    {
        return m_nodeList[((row + 1) * m_rows) + column - 1].GetTileType();
    }

    return WALL;
}

char WorldMap::GetNorthTileType(int row, int column)
{
    if (row > 0)
    {
        return m_nodeList[((row - 1) * m_rows) + column].GetTileType();
    }

    return WALL;
}

char WorldMap::GetEastTileType(int row, int column)
{
    if (column < m_columns - 1)
    {
        return m_nodeList[(row * m_rows) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthTileType(int row, int column)
{
    if (row < m_rows - 1)
    {
        return m_nodeList[((row + 1) * m_rows) + column].GetTileType();
    }

    return WALL;
}

char WorldMap::GetWestTileType(int row, int column)
{
    if (column > 0)
    {
        return m_nodeList[(row * m_rows) + (column - 1)].GetTileType();
    }

    return WALL;
}

void WorldMap::AddNode(const UINT row, const UINT column, char tileType)
{
    const int nodeIndex = (row * m_rows) + column;
    if (m_nodeList.size() < nodeIndex)
    {
        return;
    }

    m_nodeList[nodeIndex].Initialize(row, column, tileType);

    //m_worldMapLog << "Update Nodes: Row: " << row << " Column: " << column << " TileType: " << tileType << endl;
}

void WorldMap::PrintWorldMap(string context)
{
    m_worldMapLog << context << endl;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int nodeIndex = m_rows * i;
            nodeIndex += j;
            //m_worldMapLog << "Row: " << i << " Column: " << j << " Node Index: " << nodeIndex;
            m_worldMapLog << m_nodeList[nodeIndex].GetTileType();
        }

        m_worldMapLog << endl;
    }
}

void WorldMap::PrintRooms()
{
    m_worldMapLog << endl;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int nodeIndex = m_rows * i;
            nodeIndex += j;
            //m_worldMapLog << "Row: " << i << " Column: " << j << " Node Index: " << nodeIndex;
            m_worldMapLog << m_nodeList[nodeIndex].GetStructure();
        }

        m_worldMapLog << endl;
    }
}
