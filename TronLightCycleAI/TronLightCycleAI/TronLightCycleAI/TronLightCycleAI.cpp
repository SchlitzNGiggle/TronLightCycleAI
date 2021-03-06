// TronLightCycleAI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CommonDefs.h"
#include "WorldMap.h"

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>

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
        
        auto size = sscanf_s(input.c_str(), "%d %d", &columns, &rows);
        if (size < 2)
        {
            return 0;
        }

        nodeCount = rows * columns;

        // create the map
        if (!map)
        {
            map = new WorldMap(rows, columns);
        }

        std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
        map->SetTime(t);

        for (int i = 0; i < rows; i++)
        {
            // get the current
            getline(cin, input);
            
            for (auto j = 0; j < input.size(); j++)
            {
                if (input[j] != '\n')
                {
                    map->AddNode(i, j, input[j]);

                    context = "Adding Node: " + to_string(i) + " " + to_string(j) + " Input Size: " + to_string(input.size());
                    map->PrintWorldMap(context);
                }
            }
        }

        if (input.size() > 0)
        {
            turnCount++;

            //context = "Finished Initialization:" + to_string(turnCount) + " Input Size: " + to_string(input.size());
            //map->PrintWorldMap(context);

            map->PrintTime();
            map->TagRooms();
            map->TagHallway();
            map->TagIntersections();

            map->ProcessRooms();
            map->ProcessHallways();
            //map->ProcessSimplePathCosts();

            context.clear();
            //context = "Finished Initialization:" + to_string(turnCount);
            //map->PrintWorldMap(context);
            //map->PrintRooms();
        }

        int move = map->GenerateDecision();

        cout << move << endl;
    }

    return 0;
}

