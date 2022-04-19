#ifndef CIVILIZATION_GAMESTATE_TRACKER
#define CIVILIZATION_GAMESTATE_TRACKER

#include <vector>
#include "grid.hpp"
#include "units.hpp"
#include "countries.hpp"

class gamestate
{
    Grid grid;
    std::vector<country> countries;
    std::vector<unit> units;
};

extern gamestate Main;

#endif