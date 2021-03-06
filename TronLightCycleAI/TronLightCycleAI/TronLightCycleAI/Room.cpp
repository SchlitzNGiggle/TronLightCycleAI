#include "stdafx.h"

#include "CommonDefs.h"
#include "Room.h"
#include "MapNode.h"


Room::Room()
{
    id = roomID++;
    m_exitIndex.clear();
}


Room::~Room()
{
}

void Room::AddNode(int row, int column)
{
    Node* node = new Node();
    node->SetCoordinates(row, column);
    m_nodeList.push_back(*node);
}

bool Room::Contains(int row, int column)
{
    auto nodeListSize = m_nodeList.size();
    for (auto i = 0; i < nodeListSize; i++)
    {
        if (m_nodeList[i].HasSameCoordinates(row, column))
        {
            return true;
        }
    }

    return false;
}

void Room::BuildRoom(vector<MapNode>& mapNodeList, int rows, int columns, fstream& worldMapLog)
{
    int currentNodeIndex = 0;
    //worldMapLog << "Starting Size: " << m_nodeList.size() << endl;
    while (currentNodeIndex < m_nodeList.size())
    {
        int startRow = m_nodeList[currentNodeIndex].GetRow();
        int startColumn = m_nodeList[currentNodeIndex].GetColumn();
        int topIndex = ((startRow - 1) * columns) + startColumn;
        int leftIndex = (startRow * columns) + (startColumn - 1);
        int rightIndex = (startRow * columns) + (startColumn + 1);
        int bottomIndex = ((startRow + 1) * columns) + startColumn;

        //worldMapLog << "Checking Node: " << startRow - 1 << " " << startColumn << endl;
        if ( (mapNodeList[topIndex].GetStructure() == ROOM)
             && (!Contains(startRow - 1, startColumn)) )
        {
            //worldMapLog << "Adding Node: " << startRow - 1 << " " << startColumn << endl;
            AddNode(startRow - 1, startColumn);
        }

        //worldMapLog << "Checking Node: " << startRow << " " << startColumn - 1 << endl;
        if ((mapNodeList[leftIndex].GetStructure() == ROOM)
            && (!Contains(startRow, startColumn - 1)))
        {
            //worldMapLog << "Adding Node: " << startRow << " " << startColumn - 1 << endl;
            AddNode(startRow, startColumn - 1);
        }

        //worldMapLog << "Checking Node: " << startRow << " " << startColumn + 1 << endl;
        if ((mapNodeList[rightIndex].GetStructure() == ROOM)
            && (!Contains(startRow, startColumn + 1)))
        {
            //worldMapLog << "Adding Node: " << startRow << " " << startColumn + 1 << endl;
            AddNode(startRow, startColumn + 1);
        }

        //worldMapLog << "Checking Node: " << startRow + 1 << " " << startColumn << endl;
        if ((mapNodeList[bottomIndex].GetStructure() == ROOM)
            && (!Contains(startRow + 1, startColumn)))
        {
            //worldMapLog << "Adding Node: " << startRow + 1 << " " << startColumn << endl;
            AddNode(startRow + 1, startColumn);
        }

        ////////////////////////////////////////////////////////
        //////// look for exits ////////////////////////////////
        if (mapNodeList[topIndex].GetStructure() == HALLWAY)
        {
            //worldMapLog << "Exit Node: " << startRow << " " << startColumn << endl;
            m_exitIndex.push_back(currentNodeIndex);
        }

        if (mapNodeList[bottomIndex].GetStructure() == HALLWAY)
        {
            //worldMapLog << "Exit Node: " << startRow << " " << startColumn << endl;
            m_exitIndex.push_back(currentNodeIndex);
        }
        
        if (mapNodeList[leftIndex].GetStructure() == HALLWAY)
        {
            //worldMapLog << "Exit Node: " << startRow << " " << startColumn << endl;
            m_exitIndex.push_back(currentNodeIndex);
        }
        
        if (mapNodeList[rightIndex].GetStructure() == HALLWAY)
        {
            //worldMapLog << "Exit Node: " << startRow << " " << startColumn << endl;
            m_exitIndex.push_back(currentNodeIndex);
        }

        //worldMapLog << "Update Size: " << m_nodeList.size() << endl;
        currentNodeIndex++;
    }
}

void Room::PrintRoom(fstream& worldMapLog)
{
    worldMapLog << endl;
    worldMapLog << "Printing Room" << endl;

    for (auto i = 0; i < m_nodeList.size(); i++)
    {
        worldMapLog << "Room Tile: " << m_nodeList[i].GetRow() << " " << m_nodeList[i].GetColumn() << endl;
    }

    for (auto j = 0; j < m_exitIndex.size(); j++)
    {
        worldMapLog << "Exit Tile: " << m_nodeList[m_exitIndex[j]].GetRow() << " " << m_nodeList[m_exitIndex[j]].GetColumn() << endl;
    }

    worldMapLog << "Done Printing Room" << endl;
    worldMapLog << endl;
}
