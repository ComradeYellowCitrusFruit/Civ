#include <cstdint>
#include "units.hpp"

// Don't ask why it starts at sixteen, it just does.
int idTracker = 0x10;

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
        // View distance
        int view;
        // Location
        int2 position;
        // Is settler
        bool isSettler;
        // Is builder
        bool isBuilder;
        // Last attacker id
        int attackerID
        // The unit as a char
        char charRep;
        void setID(int ID)
        {
            if(id =! 0)
            {
                id = ID;
            }
        }
    private:
        // Unit ID
        int id;
};

unit Warrior;
unit Scout;
unit Slinger;
// Unlike the other templates, these ones upgrade with time
unit Builder;
unit Settler;

void initiateTemplates()
{
    Warrior.speed = 5;
    Warrior.meleeAttack = 10;
    Warrior.health = 25;
    Warrior.defense = 5;
    Warrior.range = 0;
    Warrior.rangedAttack = 0;
    Warrior.cAttack = 12;
    Warrior.view = 5;
    Warrior.isBuilder = false;
    Warrior.isSettler = false;
    Warrior.charRep = '\ua150';
    Scout.speed = 20;
    Scout.meleeAttack = 2;
    Scout.health = 15;
    Scout.defense = 5;
    Scout.range = 0;
    Scout.rangedAttack = 0;
    Scout.cAttack = 3;
    Scout.view = 15;
    Scout.isBuilder = false;
    Scout.isSettler = false;
    Scout.charRep = '\u00a7';
    Slinger.speed = 8;
    Slinger.meleeAttack = 5;
    Slinger.health = 20;
    Slinger.defense = 5;
    Slinger.range = 3;
    Slinger.rangedAttack = 10;
    Slinger.cAttack = 6;
    Slinger.view = 6;
    Slinger.isBuilder = false;
    Slinger.isSettler = false;
    Slinger.charRep = '\u1E67';
    Settler.speed = 12;
    Settler.meleeAttack = 0;
    Settler.health = 1;
    Settler.defense = 0;
    Settler.range = 0;
    Settler.rangedAttack = 0;
    Settler.cAttack = 0;
    Settler.view = 15;
    Settler.isSettler = true;
    Settler.isBuilder = false;
    Settler.charRep = '\u015C';
}