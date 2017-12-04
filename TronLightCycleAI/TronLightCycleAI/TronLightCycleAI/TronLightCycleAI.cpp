// TronLightCycleAI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonDefs.h"
#include "WorldMap.h"

#include <iostream>
#include <string>

int main()
{
    string input;
    string context;
    int rows, columns, nodeCount, turnCount;
    WorldMap* map(nullptr);

    turnCount = 0;

    while (true)
    {
        // get the map size
        getline(cin, input);
        
        sscanf_s(input.c_str(), "%d %d", &rows, &columns);
        nodeCount = rows * columns;

        // create the map
        if (!map)
        {
            map = new WorldMap(rows, columns);
        }

        for (int i = 0; i < rows; i++)
        {
            // get the current
            getline(cin, input);

            for (int j = 0; j < columns; j++)
            {
                map->AddNode(i, j, input[j]);
            }
        }

        if (input.size() > 0)
        {
            turnCount++;

            map->TagRooms();
            map->TagHallway();
            map->TagIntersections();

            map->ProcessRooms();
            map->ProcessHallways();

            context.clear();
            context = "Finished Initialization:" + to_string(turnCount);
            map->PrintWorldMap(context);
            //map->PrintRooms();
            //map->PrintHallways();
        }

        printf("%i\n", NORTH);
    }

    return 0;
}

