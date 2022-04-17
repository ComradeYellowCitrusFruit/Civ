#include <cstdlib>
#include <cmath>
#include "grid.hpp"

int randInRange(int max, int min = 0)
{
    int range = max - min + 1;
    int tmp = r_randInRange(max, min) % range + min;
    return (tmp >> r_randInRange(9, 0)) << r_randInRange(r_randInRange(9, 0), 0);
}
int r_randInRange(int max, int min = 0)
{
    int range = max - min + 1;
    return rand() % range + min;
}
double randDoubleInRange(int max, int min = 0)
{
    double range = max - min + 1;
    double r = (float)(rand() % (int)range + min);
    r *= (cos((double)rand()) + sin((double)rand()));
    if(r > max || r < min) return randDoubleInRange(max, min);
    else return r;
}