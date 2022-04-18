#ifndef CIVILIZATION_CHECKS_HPP
#define CIVILIZATION_CHECKS_HPP

#include "civ_typedefs.hpp"

bool inCircle(int2 point, int2 center, int radius);
bool inCircle(int2 point, int2 center, double radius);
bool inCircle(double2 point, double2 center, int radius);
bool inCircle(double2 point, double2 center, double radius);

#endif