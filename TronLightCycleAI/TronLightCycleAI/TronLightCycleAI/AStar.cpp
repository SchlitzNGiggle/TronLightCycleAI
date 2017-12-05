#include "stdafx.h"

#include "CommonDefs.h"
#include "AStar.h"
#include "PathNode.h"

#include <algorithm>


AStar::AStar(int rows, int columns)
{    
    m_rows = rows;
    m_columns = columns;
}


AStar::~AStar()
{
}

bool AStar::CompareNodes(const PathNode& lhs, const PathNode& rhs)
{ 
    return lhs.GetDistance() < rhs.GetDistance(); 
}

bool AStar::GetPath(int startRow, int startColumn, int endRow, int endColumn, PathNode* pathOutput)
{
    PathNode* startNode = new PathNode();
    startNode->SetCoordinates(startRow, startColumn);

    vector<PathNode> openList;
    openList.push_back(startNode);

    vector<PathNode> closedList;
    
    while (!openList.empty())
    {
        int topRow = openList[0].GetRow() - 1;
        int bottomRow = openList[0].GetRow() + 1;
        int currentRow = openList[0].GetRow();

        int leftColumn = openList[0].GetColumn() - 1;
        int rightColumn = openList[0].GetColumn() + 1;
        int currentColumn = openList[0].GetColumn();

        if (currentRow == endRow && currentColumn == currentColumn)
        {
            // node at the front of the open list is the destination
            break;
        }

        if ((topRow > 0)
            && (mapNodes[topRow * m_rows + currentColumn].GetTileType() == FLOOR)
            && !IsNodeInList(openList, topRow, currentColumn)
            && !IsNodeInList(closedList, topRow, currentColumn))
        {
            PathNode* topNode = new PathNode();
            topNode->SetCoordinates(topRow, currentColumn);
            topNode->SetPrevious(currentRow, currentColumn);
            topNode->SetManhattanDistance(CalcuateManhattanDistance(topRow, currentColumn, endRow, endColumn));
            openList.push_back(topNode);
        }

        if ((bottomRow < m_rows)
            && (mapNodes[bottomRow * m_rows + currentColumn].GetTileType() == FLOOR)
            && !IsNodeInList(openList, bottomRow, currentColumn)
            && !IsNodeInList(closedList, bottomRow, currentColumn))
        {
            PathNode* bottomNode = new PathNode();
            bottomNode->SetCoordinates(bottomRow, currentColumn);
            bottomNode->SetPrevious(currentRow, currentColumn);
            bottomNode->SetManhattanDistance(CalcuateManhattanDistance(bottomRow, currentColumn, endRow, endColumn));
            openList.push_back(bottomNode);
        }

        if ((leftColumn > 0)
            && (mapNodes[currentRow * m_rows + leftColumn].GetTileType() == FLOOR)
            && !IsNodeInList(openList, currentRow, leftColumn)
            && !IsNodeInList(closedList, currentRow, leftColumn))
        {
            PathNode* leftNode = new PathNode();
            leftNode->SetCoordinates(currentRow, leftColumn);
            leftNode->SetPrevious(currentRow, currentColumn);
            leftNode->SetManhattanDistance(CalcuateManhattanDistance(currentRow, leftColumn, endRow, endColumn));
            openList.push_back(leftNode);
        }

        if ((rightColumn > 0)
            && (mapNodes[currentRow * m_rows + rightColumn].GetTileType() == FLOOR)
            && !IsNodeInList(openList, currentRow, rightColumn)
            && !IsNodeInList(closedList, currentRow, rightColumn))
        {
            PathNode* rightNode = new PathNode();
            rightNode->SetCoordinates(currentRow, rightColumn);
            rightNode->SetPrevious(currentRow, currentColumn);
            rightNode->SetManhattanDistance(CalcuateManhattanDistance(currentRow, rightColumn, endRow, endColumn));
            openList.push_back(rightNode);
        }

        PathNode* closedNode = new PathNode();
        closedNode->SetCoordinates(currentRow, currentColumn);
        closedNode->SetPrevious(openList[0].GetPreviousRow(), openList[0].GetPreviousColumn());
        closedList.push_back(closedNode);

        openList.erase(0);

        std::sort(openList.front(), openList.back(), CompareNodes)
    }

    if (openList.size() > 0)
    {
        PathNode* pathNode = new PathNode();
        pathNode->SetCoordinates(openList[0].GetRow(), openList[0].GetColumn());
        pathNode->SetPrevious(openList[0].GetPreviousRow(), openList[0].GetPreviousColumn());
        path.push_back(pathNode);

        bool buildPath = (openList[0].GetPreviousRow() > 0) && (openList[0].GetPreviousColumn() > 0);
        while (buildPath)
        {
            for (auto i = 0; i < closedList.size(); i++)
            {
                if (closedList[i].GetRow() == path[path.size() - 1].GetPreviousRow()
                    && closedList[i].GetColumn() == path[path.size() - 1].GetColumn())
                {
                    PathNode* pathNode = new PathNode();
                    pathNode->SetCoordinates(closedList[i].GetRow(), closedList[i].GetColumn());
                    pathNode->SetPrevious(closedList[i].GetPreviousRow(), closedList[i].GetPreviousColumn());
                    path.push_back(pathNode);
                }
            }

            buildPath = (path[path.size() - 1].GetPreviousRow() > 0) && (path[path.size() - 1].GetPreviousColumn() > 0);
        }
    }

    
    return (path.size() > 0);
}

bool AStar::IsNodeInList(const vector<PathNode> nodeList, int row, int column)
{
    for (auto i = 0; i < nodeList.size(); i++)
    {
        if (nodeList[i].GetRow() == row && nodeList[i].GetColumn() == column)
        {
            return true;
        }
    }

    return false;
}

int AStar::CalcuateManhattanDistance(int currentRow, int currentColumn, int endRow, int endColumn)
{
    return abs(endRow - currentRow) + abs(endColumn - currentColumn);
}
