#include "civ_math.hpp"
#include "civ_typedefs.hpp"
#include "checks.hpp"

bool inCircle(int2 point, int2 center, int radius)
{
    int distance = (int)round(fabs((double)pythagoreanI(point.x - center.x, point.y - center.y)));
    if(distance <= radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool inCircle(int2 point, int2 center, double radius)
{
    double distance = fabs((double)pythagoreanI(point.x - center.x, point.y - center.y));
    if(distance <= radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool inCircle(double2 point, double2 center, int radius)
{
    int distance = (int)round(fabs(pythagoreanD(point.x - center.x, point.y - center.y)));
    if(distance <= radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}
bool inCircle(double2 point, double2 center, double radius)
{
    double distance = fabs(pythagoreanD(point.x - center.x, point.y - center.y));
    if(distance <= radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}