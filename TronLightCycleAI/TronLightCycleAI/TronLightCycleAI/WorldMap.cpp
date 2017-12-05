#include "stdafx.h"
#include "WorldMap.h"

#include "AStar.h"
#include "Node.h"
#include "MapNode.h"
#include "PathNode.h"

#include <algorithm>


#define LOGFILE "WorldMap.txt"

WorldMap::WorldMap(int rows, int columns)
{
    m_rows = rows;
    m_columns = columns;
    m_nodeList.reserve(rows * columns);
    m_roomList.reserve(rows * columns);
    m_hallwayList.reserve(rows * columns);
    m_player1PathCosts.reserve(m_rows * columns);
    m_player2PathCosts.reserve(m_rows * columns);

    for (int i = 0; i < rows; i++)
    {
        for (auto j = 0; j < columns; j++)
        {
            MapNode* newNode = new MapNode();
            m_nodeList.push_back(*newNode);
        }
    }

    m_worldMapLog.open(LOGFILE, ios::trunc|ios::out);
    m_worldMapLog << "Rows: " << m_rows << " Columns: " << columns << endl;
}


WorldMap::~WorldMap()
{
    m_worldMapLog.close();
}

void WorldMap::PrintTime()
{
    std::chrono::duration<double> current = m_time.time_since_epoch();
    m_worldMapLog << "The time is: " << current.count() << endl;
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
            if (m_nodeList[(i * m_columns) + j].GetTileType() == WALL)
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
                m_nodeList[(i * m_columns) + j].SetRoom();
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
                    m_nodeList[(i * m_columns) + j].SetRoom();
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
            if (m_nodeList[(i * m_columns) + j].GetTileType() == WALL)
            {
                continue;
            }

            if (m_nodeList[(i * m_columns) + j].GetStructure() == ROOM)
            {
                continue;
            }

            if (m_nodeList[(i * m_columns) + j].GetTileType() == FLOOR)
            {
                m_nodeList[(i * m_columns) + j].SetHallway();
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
            if (m_nodeList[(i * m_columns) + j].GetTileType() == WALL)
            {
                continue;
            }

            if (m_nodeList[(i * m_columns) + j].GetStructure() == INTERSECTION)
            {
                continue;
            }

            if (m_nodeList[(i * m_columns) + j].GetStructure() == ROOM)
            {
                int adjacentHallwaysCount = 0;
                if (m_nodeList[(i * m_columns) + (j - 1)].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[(i * m_columns) + (j + 1)].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[((i + 1) * m_columns) + j].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (m_nodeList[((i - 1) * m_columns) + j].GetStructure() == HALLWAY)
                {
                    adjacentHallwaysCount++;
                }

                if (adjacentHallwaysCount >= 3)
                {
                    m_nodeList[(i * m_columns) + j].SetIntersection();
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
            int realRow = i * m_columns;
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
    //for (auto i = 0; i < m_roomList.size(); i++)
    //{
    //    m_roomList[i].PrintRoom(m_worldMapLog);
    //}
}

int WorldMap::GetRoomID(int row, int column)
{
    for (auto i = 0; i < m_roomList.size(); i++)
    {
        if (m_roomList[i].Contains(row, column))
        {
            return m_roomList[i].GetID();
        }
    }
    return 0;
}

int WorldMap::GetHallwayID(int row, int column)
{
    for (auto i = 0; i < m_hallwayList.size(); i++)
    {
        //m_worldMapLog << "Test Hallway: " << i << endl;
        if (m_hallwayList[i].Contains(row, column))
        {
            //m_worldMapLog << "Found In: " << i << endl;
            return m_hallwayList[i].GetID();
        }
    }
    return 0;
}

void WorldMap::ProcessSimplePathCosts()
{
    m_player1PathCosts.clear();
    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
            PathNode* node = new PathNode();
            node->SetCoordinates(i * m_columns, j);
            node->SetCost(-1);
            m_player1PathCosts.push_back(*node);
        } 
    }

    m_worldMapLog << "Adding Player 1 Index for Node: " << m_player1PosRow << " " << m_player1PosColumn << " Index: " << m_player1PosRow * m_rows + m_player1PosColumn << endl;
    vector<PathNode> openNodeList;
    openNodeList.reserve(m_rows * m_columns);

    PathNode* player1Node = new PathNode();
    player1Node->SetCoordinates(m_player1PosRow, m_player1PosColumn);
    player1Node->SetCost(0);

    openNodeList.push_back(*player1Node);
    
    m_player1PathCosts[m_player1PosRow * m_columns + m_player1PosColumn].SetCost(0);

    int currentIndex = 0;
    while (currentIndex < openNodeList.size())
    {
        int bottomIndex = (openNodeList[currentIndex].GetRow() + 1) * m_columns + openNodeList[currentIndex].GetColumn();
        if ( (m_nodeList[bottomIndex].GetTileType() == FLOOR)
            && (m_player1PathCosts[bottomIndex].GetCost() < 0) )
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(openNodeList[currentIndex].GetRow() + 1, openNodeList[currentIndex].GetColumn());
            openNodeList.push_back(*topNode);

            m_player1PathCosts[bottomIndex].SetCost(openNodeList[currentIndex].GetCost() + 1);
        }

        int topIndex = (openNodeList[currentIndex].GetRow() - 1) * m_columns + openNodeList[currentIndex].GetColumn();
        if ( (m_nodeList[topIndex].GetTileType() == FLOOR)
            && (m_player1PathCosts[topIndex].GetCost() < 0) )
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(openNodeList[currentIndex].GetRow() - 1, openNodeList[currentIndex].GetColumn());
            openNodeList.push_back(*topNode);

            m_player1PathCosts[topIndex].SetCost(openNodeList[currentIndex].GetCost() + 1);
        }

        int rightIndex = (openNodeList[currentIndex].GetRow()) * m_columns + openNodeList[currentIndex].GetColumn() + 1;
        if ( (m_nodeList[rightIndex].GetTileType() == FLOOR)
            && (m_player1PathCosts[rightIndex].GetCost() < 0) )
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(openNodeList[currentIndex].GetRow(), openNodeList[currentIndex].GetColumn() + 1);
            openNodeList.push_back(*topNode);

            m_player1PathCosts[rightIndex].SetCost(openNodeList[currentIndex].GetCost() + 1);
        }

        int leftIndex = (openNodeList[currentIndex].GetRow()) * m_columns + openNodeList[currentIndex].GetColumn() - 1;
        if ( (m_nodeList[leftIndex].GetTileType() == FLOOR)
            && (m_player1PathCosts[leftIndex].GetCost() < 0) )
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(openNodeList[currentIndex].GetRow(), openNodeList[currentIndex].GetColumn() - 1);
            openNodeList.push_back(*topNode);

            m_player1PathCosts[leftIndex].SetCost(openNodeList[currentIndex].GetCost() + 1);
        }

        currentIndex++;
    }

    m_worldMapLog << "Printing Player1 Node Costs" << endl;
    for (auto i = 0; i < m_rows; i++)
    {
        for (auto j = 0; j < m_columns; j++)
        {
            m_worldMapLog << m_player1PathCosts[i * m_columns + j].GetCost() << " ";
        }

        m_worldMapLog << endl;
    }

    m_worldMapLog << endl;

    //m_player2PathCosts.clear();
    //for (auto i = 0; i < m_rows; i++)
    //{
    //    for (auto j = 0; j < m_columns; j++)
    //    {
    //        PathNode* node = new PathNode();
    //        node->SetCoordinates(i * m_rows, j);
    //        m_player2PathCosts.push_back(*node);
    //    }
    //}
}

int WorldMap::GenerateDecision()
{
    int player1RoomID = -1;
    int player1HallwayID = -1;
    int player1AtIntersection = false;

    int player2RoomID = -1;
    int player2HallwayID = -1;
    int player2AtIntersection = false;


    if (m_nodeList[m_player1NodeIndex].GetStructure() == ROOM)
    {
        player1RoomID = GetRoomID(m_player1PosRow, m_player1PosColumn);
    }
    else if (m_nodeList[m_player1NodeIndex].GetStructure() == HALLWAY)
    {
        player1HallwayID = GetHallwayID(m_player1PosRow, m_player1PosColumn);
    }
    else
    {
        player1AtIntersection = true;
    }

    if (m_nodeList[m_player2NodeIndex].GetStructure() == ROOM)
    {
        player2RoomID = GetRoomID(m_player2PosRow, m_player2PosColumn);
    }
    else if (m_nodeList[m_player2NodeIndex].GetStructure() == HALLWAY)
    {
        player2HallwayID = GetHallwayID(m_player2PosRow, m_player2PosColumn);
    }
    else
    {
        player2AtIntersection = true;
    }

    if (player2RoomID == player1RoomID)
    {
        bool roomHasExit = false;
        for (auto i = 0; i < m_roomList.size(); i++)
        {
            if (m_roomList[i].GetID() == player1RoomID)
            {
                if (m_roomList[i].HasExits())
                {
                    roomHasExit = true;
                }
                break;
            }
        }

        //m_worldMapLog << "Path To Player 2: " << endl;

        vector<PathNode> path;
        GetPath(m_player1PosRow, m_player1PosColumn, m_player2PosRow, m_player2PosColumn, path);

        //m_worldMapLog << "Path Found Length: " << path.size() << endl;
        int goodIndex = -1;
        for (auto i = 0; i < path.size(); i++)
        {
            //m_worldMapLog << "Path Row: " << path[i].GetRow() << " Path Column: " << path[i].GetColumn() << endl;
            if (path[i].GetRow() == m_player1PosRow && path[i].GetColumn() == m_player1PosColumn)
            {
                goodIndex = i - 1;
                //m_worldMapLog << "Found good index: " << goodIndex << endl;
                //m_worldMapLog << "Row: " << path[goodIndex].GetRow() << " Column: " << path[goodIndex].GetColumn() << endl;
                break;
            }
        }

        //if (path.size() > 0)
        //{
        //    //m_worldMapLog << "Print Path" << endl;
        //    for (auto i = 0; i < m_rows; i++)
        //    {
        //        for (auto j = 0; j < m_columns; j++)
        //        {
        //            int nodeIndex = m_rows * i;
        //            nodeIndex += j;

        //            bool nodeFoundInPath = false;
        //            for (auto k = 0; k < path.size(); k++)
        //            {
        //                if ((m_nodeList[nodeIndex].GetRow() == path[k].GetRow())
        //                    && (m_nodeList[nodeIndex].GetColumn() == path[k].GetColumn()))
        //                {
        //                    nodeFoundInPath = true;
        //                    //m_worldMapLog << 'D';
        //                    break;
        //                }
        //            }

        //            if (!nodeFoundInPath)
        //            {
        //                m_worldMapLog << m_nodeList[nodeIndex].GetTileType();
        //            }

        //        }

        //        m_worldMapLog << endl;
        //    }
        //}

        if (path.size() >= 5 && goodIndex < (path.size() - 1) && goodIndex > 0)
        {
            if (path[goodIndex].GetRow() > m_player1PosRow)
            {
                //m_worldMapLog << "Move South" << endl;
                return SOUTH;
            }
            else if (path[goodIndex].GetRow() < m_player1PosRow)
            {
                //m_worldMapLog << "Move North" << endl;
                return NORTH;
            }
            else if (path[goodIndex].GetColumn() > m_player1PosColumn)
            {
                //m_worldMapLog << "Move East" << endl;
                return EAST;
            }
            else
            {
                //m_worldMapLog << "Move West" << endl;
                return WEST;
            }
        }
        else if (path.size() == 0)
        {
            std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
            std::chrono::duration<double> current = t.time_since_epoch();
            m_worldMapLog << "Current Time: " << current.count() << endl;

            vector<PathNode> tempPath;
            tempPath.reserve(m_rows * m_columns);
            while(true)
            {
                tempPath.clear();
                PathNode* startNode = new PathNode();
                startNode->SetCoordinates(m_player1PosRow, m_player1PosColumn);
                tempPath.push_back(*startNode);

                bool nodeFound = true;
                while (nodeFound)
                {
                    nodeFound = false;

                    auto tempPathSize = tempPath.size();
                    vector<char> validDir;
                    validDir.clear();

                    int tempRow = tempPath[tempPathSize - 1].GetRow();
                    int tempColumn = tempPath[tempPathSize - 1].GetColumn();
                    if ((GetNorthTileType(tempRow, tempColumn) == FLOOR)
                        && !IsNodeInList(tempPath, tempRow - 1, tempColumn))
                    {
                        //m_worldMapLog << "Adding node: " << tempRow - 1 << " " << tempColumn << endl;
                        validDir.push_back(NORTH);
                    }

                    if ((GetSouthTileType(tempRow, tempColumn) == FLOOR)
                        && !IsNodeInList(tempPath, tempRow + 1, tempColumn))

                    {
                        //m_worldMapLog << "Adding node: " << tempRow + 1 << " " << tempColumn << endl;
                        validDir.push_back(SOUTH);
                    }

                    if ((GetEastTileType(tempRow, tempColumn) == FLOOR)
                        && !IsNodeInList(tempPath, tempRow, tempColumn + 1))

                    {
                        //m_worldMapLog << "Adding node: " << tempRow << " " << tempColumn + 1 << endl;
                        validDir.push_back(EAST);
                    }

                    if ((GetWestTileType(tempRow, tempColumn) == FLOOR)
                        && !IsNodeInList(tempPath, tempRow, tempColumn - 1))
                    {
                        //m_worldMapLog << "Adding node: " << tempRow << " " << tempColumn - 1 << endl;
                        validDir.push_back(WEST);
                    }

                    if (validDir.size() > 0)
                    {
                        nodeFound = true;
                        PathNode* newNode = new PathNode();
                        int selection = rand() % validDir.size();
                        //m_worldMapLog << "Selection is: " << selection << endl;

                        char direction = validDir[selection];

                        //m_worldMapLog << "Direction is: " << direction << endl;
                        switch (direction)
                        {

                        case SOUTH:
                        {
                            newNode->SetCoordinates(tempRow + 1, tempColumn);
                        }
                        break;

                        case WEST:
                        {
                            newNode->SetCoordinates(tempRow, tempColumn - 1);
                        }
                        break;

                        case EAST:
                        {
                            newNode->SetCoordinates(tempRow, tempColumn + 1);
                        }
                        break;


                        case NORTH:
                        {
                            newNode->SetCoordinates(tempRow - 1, tempColumn);
                        }
                        break;

                        }

                        tempPath.push_back(*newNode);
                    }
                }

                if (tempPath.size() > path.size())
                {
                    path = tempPath;
                }

                std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
                std::chrono::duration<double> current = t.time_since_epoch();

                std::chrono::duration<double> diff = t - m_time;
                if (diff.count() > 0.95)
                {
                    m_worldMapLog << "Diff from start to large, force break: " << diff.count() << endl;
                    break;
                }
            }

            if (path.size() > 1)
            {
                //m_worldMapLog << "Path Found Length: " << path.size() << endl;
                int goodIndex = -1;
                for (auto i = 0; i < path.size(); i++)
                {
                    //m_worldMapLog << "Path Row: " << path[i].GetRow() << " Path Column: " << path[i].GetColumn() << endl;
                    if (path[i].GetRow() == m_player1PosRow && path[i].GetColumn() == m_player1PosColumn)
                    {
                        goodIndex = i + 1;
                        //m_worldMapLog << "Found good index: " << goodIndex << endl;
                        //m_worldMapLog << "Row: " << path[goodIndex].GetRow() << " Column: " << path[goodIndex].GetColumn() << endl;
                        break;
                    }
                }

                //m_worldMapLog << "Print Path" << endl;
                //for (auto i = 0; i < m_rows; i++)
                //{
                //    for (auto j = 0; j < m_columns; j++)
                //    {
                //        int nodeIndex = m_rows * i;
                //        nodeIndex += j;

                //        bool nodeFoundInPath = false;
                //        for (auto k = 0; k < path.size(); k++)
                //        {
                //            if ((m_nodeList[nodeIndex].GetRow() == path[k].GetRow())
                //                && (m_nodeList[nodeIndex].GetColumn() == path[k].GetColumn()))
                //            {
                //                nodeFoundInPath = true;
                //                m_worldMapLog << 'D';
                //                break;
                //            }
                //        }

                //        if (!nodeFoundInPath)
                //        {
                //            m_worldMapLog << m_nodeList[nodeIndex].GetTileType();
                //        }

                //    }

                //    m_worldMapLog << endl;
                //}

                if ((goodIndex > 0) && (goodIndex < path.size() - 1))
                {
                    if (path[goodIndex].GetRow() > m_player1PosRow)
                    {
                        //m_worldMapLog << "Move South" << endl;
                        return SOUTH;
                    }
                    else if (path[goodIndex].GetRow() < m_player1PosRow)
                    {
                        // m_worldMapLog << "Move North" << endl;
                        return NORTH;
                    }
                    else if (path[goodIndex].GetColumn() > m_player1PosColumn)
                    {
                        //m_worldMapLog << "Move East" << endl;
                        return EAST;
                    }
                    else
                    {
                        //m_worldMapLog << "Move West" << endl;
                        return WEST;
                    }
                }
            }
            else
            {
                vector<char> validDir;
                if ((GetNorthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                    && !CanPlayer2ReachTile(m_player1PosRow - 1, m_player1PosColumn))
                {
                    validDir.push_back(NORTH);
                }
                if ((GetSouthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                    && !CanPlayer2ReachTile(m_player1PosRow + 1, m_player1PosColumn))
                {
                    validDir.push_back(SOUTH);
                }
                if ((GetEastTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                    && !CanPlayer2ReachTile(m_player1PosRow, m_player1PosColumn + 1))
                {
                    validDir.push_back(EAST);
                }
                if ((GetWestTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                    && !CanPlayer2ReachTile(m_player1PosRow, m_player1PosColumn - 1))
                {
                    validDir.push_back(WEST);
                }

                if (validDir.size() > 0)
                {
                    return validDir[rand() % validDir.size()];
                }

                //vector<char> validDir;
                if (GetNorthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                {
                    validDir.push_back(NORTH);
                }
                if (GetSouthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)

                {
                    validDir.push_back(SOUTH);
                }
                if (GetEastTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)

                {
                    validDir.push_back(EAST);
                }
                if (GetWestTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)

                {
                    validDir.push_back(WEST);
                }

                if (validDir.size() > 0)
                {
                    return validDir[rand() % validDir.size()];
                }
            }
            
        }
        else
        {
            vector<char> validDir;
            if ((GetNorthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                && !CanPlayer2ReachTile(m_player1PosRow - 1, m_player1PosColumn))
            {
                validDir.push_back(NORTH);
            }
            if ((GetSouthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                && !CanPlayer2ReachTile(m_player1PosRow + 1, m_player1PosColumn))
            {
                validDir.push_back(SOUTH);
            }
            if ((GetEastTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                && !CanPlayer2ReachTile(m_player1PosRow, m_player1PosColumn + 1))
            {
                validDir.push_back(EAST);
            }
            if ((GetWestTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                && !CanPlayer2ReachTile(m_player1PosRow, m_player1PosColumn - 1))
            {
                validDir.push_back(WEST);
            }

            if (validDir.size() > 0)
            {
                return validDir[rand() % validDir.size()];
            }

            //vector<char> validDir;
            if (GetNorthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)               
            {
                validDir.push_back(NORTH);
            }
            if (GetSouthTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
                
            {
                validDir.push_back(SOUTH);
            }
            if (GetEastTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
               
            {
                validDir.push_back(EAST);
            }
            if (GetWestTileType(m_player1PosRow, m_player1PosColumn) == FLOOR)
               
            {
                validDir.push_back(WEST);
            }

            if (validDir.size() > 0)
            {
                return validDir[rand() % validDir.size()];
            }
        }
    }

    return NORTH;
}

bool WorldMap::CanPlayer2ReachTile(int row, int columns)
{
    if (row == m_player2PosRow)
    {
        if (m_player2PosColumn + 1 == columns)
        {
            return true;
        }

        if (m_player2PosColumn - 1 == columns)
        {
            return true;
        }
    }
    else if (columns == m_player2PosColumn)
    {
        if (m_player2PosRow + 1 == row)
        {
            return true;
        }

        if (m_player2PosRow - 1 == row)
        {
            return true;
        }
    }

    return false;
}

bool WorldMap::GetPath(int startRow, int startColumn, int endRow, int endColumn, vector<PathNode>& path)
{
    vector<PathNode> openList;
    vector<PathNode> closedList;
    openList.reserve(m_rows * m_columns);
    closedList.reserve(m_rows * m_columns);

    PathNode* startNode = new PathNode();
    startNode->SetCoordinates(startRow, startColumn);
    startNode->SetCost(0);
    startNode->SetManhattanDistance(GetDistance(startRow, startColumn, endRow, endColumn));
    startNode->SetPreviousNode(nullptr);
    openList.push_back(*startNode);

    //m_worldMapLog << "Goal Location is " << endRow << " " << endColumn << endl;

    //m_worldMapLog << "Fist Node Added" << startRow << " " << startColumn << endl;

    while (!openList.empty())
    {
        int currentRow = openList[0].GetRow();
        int currentColumn = openList[0].GetColumn();

        if (currentRow == endRow && currentColumn == endColumn)
        {
            // path found
            //m_worldMapLog << "Found Goal Node" << endl;
            break;
        }

        PathNode closedNode = openList[0];
        closedList.push_back(closedNode);

        int topRow = openList[0].GetRow() - 1;
        int bottomRow = openList[0].GetRow() + 1;

        int leftColumn = openList[0].GetColumn() - 1;
        int rightColumn = openList[0].GetColumn() + 1;

        char northTile = GetNorthTileType(currentRow, currentColumn);
        if ((topRow > 0)
            &&  (northTile == FLOOR || northTile == PLAYER1 || northTile == PLAYER2)
            && !IsNodeInList(openList, topRow, currentColumn)
            && !IsNodeInList(closedList, topRow, currentColumn))
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(topRow, currentColumn);
            topNode->SetManhattanDistance(GetDistance(topRow, currentColumn, endRow, endColumn));
            topNode->SetPrevious(currentRow, currentColumn);
            topNode->SetPreviousNode(&closedNode);
            openList.push_back(*topNode);

            //m_worldMapLog << "Upper Node Added: " << topRow << " " << currentColumn << endl;
        }

        char southTile = GetSouthTileType(currentRow, currentColumn);
        if ((bottomRow < m_rows)
            && (southTile == FLOOR || southTile == PLAYER1 || southTile == PLAYER2)
            && !IsNodeInList(openList, bottomRow, currentColumn)
            && !IsNodeInList(closedList, bottomRow, currentColumn))
        {
            PathNode* bottomNode = new PathNode();
            bottomNode->SetCoordinates(bottomRow, currentColumn);
            bottomNode->SetManhattanDistance(GetDistance(bottomRow, startColumn, endRow, endColumn));
            bottomNode->SetPrevious(currentRow, currentColumn);
            bottomNode->SetPreviousNode(&closedNode);
            openList.push_back(*bottomNode);

            //m_worldMapLog << "South Node Added: " << bottomRow << " " << currentColumn << endl;
        }

        char westTile = GetWestTileType(currentRow, currentColumn);
        if ((leftColumn > 0)
            && (westTile == FLOOR || westTile == PLAYER1 || westTile == PLAYER2)
            && !IsNodeInList(openList, currentRow, leftColumn)
            && !IsNodeInList(closedList, currentRow, leftColumn))
        {
            PathNode* leftNode = new PathNode();
            leftNode->SetCoordinates(currentRow, leftColumn);
            leftNode->SetManhattanDistance(GetDistance(currentRow, leftColumn, endRow, endColumn));
            leftNode->SetPrevious(currentRow, currentColumn);
            leftNode->SetPreviousNode(&closedNode);
            openList.push_back(*leftNode);

            //m_worldMapLog << "West Node Added: " << currentRow << " " << leftColumn << endl;
        }

        char eastTile = GetEastTileType(currentRow, currentColumn);
        if ((rightColumn < m_columns)
            && (eastTile == FLOOR || eastTile == PLAYER1 || eastTile == PLAYER2)
            && !IsNodeInList(openList, currentRow, rightColumn)
            && !IsNodeInList(closedList, currentRow, rightColumn))
        {
            PathNode* rightNode = new PathNode();
            rightNode->SetCoordinates(currentRow, rightColumn);
            rightNode->SetManhattanDistance(GetDistance(currentRow, rightColumn, endRow, endColumn));
            rightNode->SetPrevious(currentRow, currentColumn);
            rightNode->SetPreviousNode(&closedNode);
            openList.push_back(*rightNode);

            //m_worldMapLog << "East Node Added: " << currentRow << " " << rightColumn << endl;
        }

        openList.erase(openList.begin());

        std::sort(openList.begin(), openList.end(), CompareNode());
    }

    // build path
    if (openList.size() > 0)
    {
        //m_worldMapLog << "Started Building Path" << endl;
        
        PathNode* nextNode = &openList[0];
        path.push_back(openList[0]);

        //m_worldMapLog << "End Node Is: " << path[0].GetRow() << " " << path[0].GetColumn() << endl;
        bool nodeFound = true;
        while (nextNode && nodeFound)
        {
            nodeFound = false;
            for (auto i = 0; i < closedList.size(); i++)
            {
                if ((nextNode->GetPreviousRow() == closedList[i].GetRow())
                    && (nextNode->GetPreviousColumn() == closedList[i].GetColumn()))
                {
                    //m_worldMapLog << "Node In Closed List"  << endl;
                    nextNode = &closedList[i];
                    nodeFound = true;
                    break;
                }
            }

            path.push_back(*nextNode);
            //m_worldMapLog << "Current Node Is: " << path[path.size() - 1].GetRow() << " " << path[path.size() - 1].GetColumn() << endl; 
        }

        //m_worldMapLog << "Finished Building Path" << endl;
    }

    return false;
}

bool WorldMap::IsNodeInList(const vector<PathNode>& list, int row, int column)
{
    for (auto i = 0; i < list.size(); i++)
    {
        if ((list[i].GetRow() == row) && (list[i].GetColumn() == column))
        {
            return true;
        }
    }

    return false;
}

int WorldMap::GetDistance(int startRow, int startColumn, int endRow, int endColumn)
{
    return abs(endRow - startRow) + abs(startColumn - endColumn);
}

void WorldMap::ProcessHallways()
{
    m_hallwayList.clear();
    //m_worldMapLog << "Trying to build hallways" << endl;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int realRow = i * m_columns;
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
        return m_nodeList[((row - 1) * m_columns) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetNorthWestTileType(int row, int column)
{
    if ((row > 0) && (column > 0))
    {
        return m_nodeList[((row - 1) * m_columns) + (column - 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthEastTileType(int row, int column)
{
    if ((row < m_rows - 1) && (column < m_columns - 1))
    {
        return m_nodeList[((row + 1) * m_columns) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthWestTileType(int row, int column)
{
    if ((row < m_rows - 1) && (column > 0))
    {
        return m_nodeList[((row + 1) * m_columns) + column - 1].GetTileType();
    }

    return WALL;
}

char WorldMap::GetNorthTileType(int row, int column)
{
    if (row > 0)
    {
        return m_nodeList[((row - 1) * m_columns) + column].GetTileType();
    }

    return WALL;
}

char WorldMap::GetEastTileType(int row, int column)
{
    if (column < m_columns - 1)
    {
        return m_nodeList[(row * m_columns) + (column + 1)].GetTileType();
    }

    return WALL;
}

char WorldMap::GetSouthTileType(int row, int column)
{
    if (row < m_rows - 1)
    {
        return m_nodeList[((row + 1) * m_columns) + column].GetTileType();
    }

    return WALL;
}

char WorldMap::GetWestTileType(int row, int column)
{
    if (column > 0)
    {
        return m_nodeList[(row * m_columns) + (column - 1)].GetTileType();
    }

    return WALL;
}

void WorldMap::AddNode(const UINT row, const UINT column, char tileType)
{
    const int nodeIndex = (row * m_columns) + column;
    if (m_nodeList.size() < nodeIndex)
    {
        return;
    }

    m_nodeList[nodeIndex].Initialize(row, column, tileType);

    if (tileType == PLAYER1)
    {
        m_player1PosRow = row;
        m_player1PosColumn = column;
        m_player1NodeIndex = nodeIndex;
    }
    else if (tileType == PLAYER2)
    {
        m_player2PosRow = row;
        m_player2PosColumn = column;
        m_player2NodeIndex = nodeIndex;
    }

    //m_worldMapLog << "Update Nodes: Row: " << row << " Column: " << column << " TileType: " << tileType << endl;
}

void WorldMap::PrintWorldMap(string context)
{
    m_worldMapLog << context << endl;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            int nodeIndex = m_columns * i;
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
