#include <vector>
#include "gamestate.hpp"
#include "grid.hpp"
#include "units.hpp"
#include "countries.hpp"

class gamestate
{
    Grid grid;
    std::vector<country> countries;
    std::vector<unit> units;
};

gamestate Main;