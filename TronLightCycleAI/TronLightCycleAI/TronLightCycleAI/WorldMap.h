#pragma once
#include "CommonDefs.h"
#include "Node.h"
#include "MapNode.h"
#include "Room.h"
#include "Hallway.h"

using namespace std;

class WorldMap
{
public:
    WorldMap(int rows, int columns);
    ~WorldMap();

    void AddNode(const UINT row, const UINT column, char tileType);
    void TagRooms();
    void TagHallway();
    void TagIntersections();

    void ProcessRooms();
    void ProcessHallways();

    void PrintWorldMap(string context);
    void PrintRooms();

    int GetRowCount() const { return m_rows; }
    int GetColumnCount() const { return m_columns; }

private:
    vector<MapNode> m_nodeList;
    vector<Room> m_roomList;
    vector<Hallway> m_hallwayList;

    char GetNorthTileType(int row, int column);
    char GetEastTileType(int row, int column);
    char GetSouthTileType(int row, int column);
    char GetWestTileType(int row, int column);
    char GetNorthEastTileType(int row, int column);
    char GetNorthWestTileType(int row, int column);
    char GetSouthEastTileType(int row, int column);
    char GetSouthWestTileType(int row, int column);

    bool IsTileInRoomList(int row, int column);
    bool IsTileInHallwayList(int row, int column);

    int m_rows;
    int m_columns;

    fstream m_worldMapLog;
};

