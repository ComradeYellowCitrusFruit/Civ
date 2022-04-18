#include <cmath>
#include "civ_math.hpp"


// Rounding, please note, this does not work on negative numbers
double round(double x)
{
    double dec = x - (int)x;
    if(dec >= .5)
    {
        return x - dec + 1;
    }
    else
    {
        return x - dec;
    }
}

double pythagoreanD(double a, double b)
{
    return sqrt(pow(a, 2) + pow(b, 2));
}
int pythagoreanI(int a, int b)
{
    return (int)round(sqrt(pow((double)a, 2) + pow((double)b, 2)));
}