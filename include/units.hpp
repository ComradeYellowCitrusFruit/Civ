#ifndef CIVILIZATION_UNITS_HPP
#define CIVILIZATION_UNITS_HPP

#include <cstdint>
#include "civ_typedefs.hpp"

extern int idTracker;

class unit
{
    public:
        // Owner
        int owner;
        // Speed in tiles per turn
        int speed;
        // Can move
        bool canMove;
        // Health
        int health;
        // Melee attack
        int meleeAttack;
        // Ranged attack
        int rangedAttack;
        // Ranged attack range
        int range;
        // Counter attack
        int cAttack;
        // Defense
        int defense;
        // Terrain boosts
        uint8_t terrain;
        // Location
        int2 position;
        // Is settler
        bool isSettler;
        // Is builder
        bool isBuilder;
        // The unit as a char
        char asChar;
        void setID(int ID);
    private:
        int id;
};

extern unit Warrior;
extern unit Scout;
extern unit Slinger;
// Unlike the other templates, these ones upgrade with time
extern unit Builder;
extern unit Settler;

void initiateTemplates();

#endif