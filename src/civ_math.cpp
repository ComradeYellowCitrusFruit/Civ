#include <cmath>
#include "civ_math.hpp"

double pythagoreanD(double a, double b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}
int pythagoreanI(int a, int b)
{
    return (int)sqrt(pow((double)a, 2) + pow((double)b, 2));
}