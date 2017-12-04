#include "stdafx.h"
#include "CommonDefs.h"
#include "Hallway.h"
#include "MapNode.h"

Hallway::Hallway()
{
    id = hallwayID++;
}


Hallway::~Hallway()
{
}

void Hallway::AddNode(int row, int column)
{
    Node* node = new Node();
    node->SetCoordinates(row, column);
    m_hallwayNodes.push_back(*node);
}

bool Hallway::Contains(int row, int column)
{
    auto nodeListSize = m_hallwayNodes.size();
    for (auto i = 0; i < nodeListSize; i++)
    {
        if (m_hallwayNodes[i].HasSameCoordinates(row, column))
        {
            return true;
        }
    }

    return false;
}

void Hallway::BuildHallway(vector<MapNode>& mapNodeList, int rows, int columns, fstream& worldMapLog)
{
    int currentNodeIndex = 0;
    //worldMapLog << "Starting Size: " << m_hallwayNodes.size() << endl;
    while (currentNodeIndex < m_hallwayNodes.size())
    {
        int startRow = m_hallwayNodes[currentNodeIndex].GetRow();
        int startColumn = m_hallwayNodes[currentNodeIndex].GetColumn();
        int topIndex = ((startRow - 1) * rows) + startColumn;
        int leftIndex = (startRow * rows) + (startColumn - 1);
        int rightIndex = (startRow * rows) + (startColumn + 1);
        int bottomIndex = ((startRow + 1) * rows) + startColumn;

        if ((mapNodeList[topIndex].GetStructure() == HALLWAY)
            && (!Contains(startRow - 1, startColumn)))
        {
            AddNode(startRow - 1, startColumn);
        }

        if ((mapNodeList[leftIndex].GetStructure() == HALLWAY)
            && (!Contains(startRow, startColumn - 1)))
        {
            AddNode(startRow, startColumn - 1);
        }

        if ((mapNodeList[rightIndex].GetStructure() == HALLWAY)
            && (!Contains(startRow, startColumn + 1)))
        {
            AddNode(startRow, startColumn + 1);
        }

        if ((mapNodeList[bottomIndex].GetStructure() == HALLWAY)
            && (!Contains(startRow + 1, startColumn)))
        {
            AddNode(startRow + 1, startColumn);
        }

        currentNodeIndex++;
    }
}

void Hallway::PrintHallway(fstream& worldMapLog)
{
    worldMapLog << endl;
    worldMapLog << "Printing Hallway" << endl;

    //for (auto i = 0; i < m_nodeList.size(); i++)
    {
        worldMapLog << "Starting Tile: " << m_hallwayNodes[0].GetRow() << " " << m_hallwayNodes[0].GetColumn() << endl;
        worldMapLog << "Ending Tile: " << m_hallwayNodes[m_hallwayNodes.size() - 1].GetRow() << " " << m_hallwayNodes[m_hallwayNodes.size() - 1].GetColumn() << endl;
    }

    worldMapLog << "Done Printing Hallway" << endl;
    worldMapLog << endl;
}
