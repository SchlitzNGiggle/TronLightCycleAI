#pragma once

#include <wtypes.h>
#include <vector>
#include <fstream>
#include <string>

#define NORTH           1
#define EAST            2
#define SOUTH           3
#define WEST            4

#define WALL            '#'
#define FLOOR           ' '
#define PLAYER1         '1'
#define PLAYER2         '2'

#define ROOM            'R'
#define HALLWAY         'H'
#define INTERSECTION    'I'

enum structureType
{
    NONE,
    RoomTile,
    HallwayTile,
    Intersection,
};