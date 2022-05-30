#ifndef CIVILIZATION_TYPE_DEFINITIONS_HPP
#define CIVILIZATION_TYPE_DEFINITIONS_HPP

class double2
{
    double x;
    double y;
    double2 operator+(const double2 d);
    double2 operator+=(const double2 d);
    double2 operator+(const double d);
    double2 operator+=(const double d);
};

class double3
{
    double x;
    double y;
    double z;
    double3 operator+(const double3 d);
    double3 operator+=(const double3 d);
    double3 operator+(const double d);
    double3 operator+=(const double d);
};

class double4
{
    public:
    double w;
    double x;
    double y;
    double z;
    double4 operator+(const double4 d);
    double4 operator+=(const double4 d);
    double4 operator+(const double d);
    double4 operator+=(const double d);
};

class int2
{
    public:
    int2();
    int2(int a, int b);
    int x;
    int y;
    int2 operator+(const int2 d);
    int2 operator+=(const int2 d);
    int2 operator+(const int d);
    int2 operator+=(const int d);
};

class int3
{
    public:
    int3();
    int3(int a, int b, int c);
    int x;
    int y;
    int z;
    int3 operator+(const int3 d);
    int3 operator+=(const int3 d);
    int3 operator+(const int d);
    int3 operator+=(const int d);
};

class int4
{
    public:
    int4();
    int4(int a, int b, int c, int d);
    int w;
    int x;
    int y;
    int z;
    int4 operator+(const int4 d);
    int4 operator+=(const int4 d);
    int4 operator+(const int d);
    int4 operator+=(const int d);
};

#endif