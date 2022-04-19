#ifndef CIVILIZATION_COUNTRIES_HPP
#define CIVILIZATION_COUNTRIES_HPP

#include <vector>
#include "units.hpp"

// Each of these reserved IDs are for various barbarian groups, each ID can be used for a different 'clan' to give them some traits and make them attack each other
#define BARBARIANS_RESERVED_ID_1 0x0
#define BARBARIANS_RESERVED_ID_2 0x1
#define BARBARIANS_RESERVED_ID_3 0x2
#define BARBARIANS_RESERVED_ID_4 0x3
#define BARBARIANS_RESERVED_ID_5 0x4
#define BARBARIANS_RESERVED_ID_6 0x5
#define BARBARIANS_RESERVED_ID_7 0x6
#define BARBARIANS_RESERVED_ID_8 0x7
#define BARBARIANS_RESERVED_ID_9 0x8
#define BARBARIANS_RESERVED_ID_10 0x9
#define BARBARIANS_RESERVED_ID_11 0xA
#define BARBARIANS_RESERVED_ID_12 0xB
#define BARBARIANS_RESERVED_ID_13 0xC
#define BARBARIANS_RESERVED_ID_14 0xD
#define BARBARIANS_RESERVED_ID_15 0xE
#define BARBARIANS_RESERVED_ID_16 0xF

// Reserved ID for the player
#define PLAYER_RESERVED_ID 0x10

// Inclusive point where country IDs start, set to 32 instead of 17 for the sake of looks
#define COUNTY_ID_START_POINT 0x20

// Inclusive point where city state IDs start
#define CITY_STATE_ID_START_POINT 0xC8

void barbarianMovement(int barbarianID);

class country
{
    // County ID
    int id;
    // Units
    std::vector<unit> Units;
    // Gold
    int gold;
};

#endif