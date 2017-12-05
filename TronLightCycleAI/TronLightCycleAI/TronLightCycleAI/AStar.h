#pragma once

#include <vector>

class PathNode;

class AStar
{
public:
    AStar(int rows, int columns);
    ~AStar();

    bool GetPath(int startRow, int startColumn, int endRow, int endColumn, PathNode* path);

private:
    
    int CalcuateManhattanDistance(int currentRow, int currentColumn, int endRow, int endColumn);
    bool CompareNodes(const PathNode& lhs, const PathNode& rhs) { return lhs.GetDistance() < rhs.GetDistance(); }

    int m_rows;
    int m_columns;
};

