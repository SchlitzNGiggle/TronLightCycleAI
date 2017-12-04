#pragma once
#include "CommonDefs.h"

#include "Node.h"
#include "MapNode.h"
#include "Room.h"
#include "Hallway.h"
#include "PathNode.h"

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
    void ProcessSimplePathCosts();

    void PrintWorldMap(string context);
    void PrintRooms();

    int GetRowCount() const { return m_rows; }
    int GetColumnCount() const { return m_columns; }

    int GenerateDecision();

private:
    vector<MapNode> m_nodeList;
    vector<Room> m_roomList;
    vector<Hallway> m_hallwayList;
    vector<PathNode> m_player1PathCosts;
    vector<PathNode> m_player2PathCosts;

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

    int GetRoomID(int row, int column);
    int GetHallwayID(int row, int column);

    int m_rows;
    int m_columns;

    int m_player1PosRow;
    int m_player1PosColumn;
    int m_player1NodeIndex;

    int m_player2PosRow;
    int m_player2PosColumn;
    int m_player2NodeIndex;

    fstream m_worldMapLog;
};

