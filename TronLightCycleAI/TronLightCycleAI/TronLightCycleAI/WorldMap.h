#pragma once
#include "CommonDefs.h"

#include "Node.h"
#include "MapNode.h"
#include "Room.h"
#include "Hallway.h"
#include "PathNode.h"
#include <chrono>

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
    bool GetPath(int startRow, int startColumn, int endRow, int endColumn, vector<PathNode>& path);
    int GetDistance(int startRow, int startColumn, int endRow, int endColumn);

    void SetTime(std::chrono::time_point<std::chrono::system_clock> time) { m_time = time; }
    void PrintTime();

    const vector<Room>& GetRooms() { return m_roomList; }
    const vector<Hallway>& GetHallways() { return m_hallwayList; }
    const vector<MapNode> GetNodeList() { return m_nodeList; }

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
    bool IsNodeInList(const vector<PathNode>& list, int row, int column);
    bool CanPlayer2ReachTile(int row, int columns);

    struct CompareNode
    {
        bool operator()(const PathNode& lhs, const PathNode &rhs) const
        {
            return lhs.GetDistance() < rhs.GetDistance();
        }
    };

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

    std::chrono::time_point<std::chrono::system_clock> m_time;

    fstream m_worldMapLog;
};

