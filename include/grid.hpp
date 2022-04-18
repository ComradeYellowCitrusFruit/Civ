#ifndef CIVILIZATION_GRID_HPP
#define CIVILIZATION_GRID_HPP

#include <cstdint>
#include <mutex>

#define VIEWGRID_SIZE 80

#define WORLD_SIZES 0x5
#define SUPER_SMALL 0x50
#define SMALL 0xA0
#define MEDIUM 0xF0
#define LARGE 0x190
#define SUPER_LARGE 0x230

#define SUPER_SMALL_OCEANS_MIN 0x2
#define SUPER_SMALL_OCEANS_MAX 0x5
#define SUPER_SMALL_OCEANS_RATIO_MIN 0x3
#define SUPER_SMALL_OCEANS_RATIO_MAX 0x6
#define SUPER_SMALL_OCEANS_RADIUS_MIN 0x5
#define SUPER_SMALL_OCEANS_RADIUS_MAX 0xA

#define SMALL_OCEANS_MIN 0x4
#define SMALL_OCEANS_MAX 0xA
#define SMALL_OCEANS_RATIO_MIN 0x6
#define SMALL_OCEANS_RATIO_MAX 0xC
#define SMALL_OCEANS_RADIUS_MIN 0xA
#define SMALL_OCEANS_RADIUS_MAX 0x14

#define MEDIUM_OCEANS_MIN 0x4
#define MEDIUM_OCEANS_MAX 0xA
#define MEDIUM_OCEANS_RATIO_MIN 0xC
#define MEDIUM_OCEANS_RATIO_MAX 0x18
#define MEDIUM_OCEANS_RADIUS_MIN 0xF
#define MEDIUM_OCEANS_RADIUS_MAX 0x1E

#define LARGE_OCEANS_MIN 0x8
#define LARGE_OCEANS_MAX 0x14
#define LARGE_OCEANS_RATIO_MIN 0x12
#define LARGE_OCEANS_RATIO_MAX 0x24
#define LARGE_OCEANS_RADIUS_MIN 0x16
#define LARGE_OCEANS_RADIUS_MAX 0x2D

#define SUPER_LARGE_OCEANS_MIN 0xC
#define SUPER_LARGE_OCEANS_MAX 0x1E
#define SUPER_LARGE_OCEANS_RATIO_MIN 0x1B
#define SUPER_LARGE_OCEANS_RATIO_MAX 0x36
#define SUPER_LARGE_OCEANS_RADIUS_MIN 0x21
#define SUPER_LARGE_OCEANS_RADIUS_MAX 0x44

#ifndef GRID_PROCESSING_CHUNK_SIZE
#define GRID_PROCESSING_CHUNK_SIZE 0x50
#endif

#ifndef PERLIN_NOISE_FREQUENCY
#define PERLIN_NOISE_FREQUENCY 0x10
#endif

#ifndef PERLIN_NOISE_DEPTH
#define PERLIN_NOISE_DEPTH 0x12
#endif

/*  The grid for handling the map
*   It contains functions for rendering and modifying the grid/map
*   There should be only one grid per world
*/
class Grid
{
    public:
    // X size
    int xSize;
    // Y size
    int ySize;
    // Actual squares, initalized to a 2d array of varying size
    gridSquare **squares;
    // Don't use this constructor, don't. I just made this to get vscode to shut the fuck up
    Grid();
    // Actual constructor
    Grid(int size);
    // Self explanitory, renders the grid
    renderedGrid render();
    private:
    // Generates the world
    void generateWorld();
    // Multithreading function to generate the oceans.
    void thrOceans(double **oceanGrid, int oceanRatio, int oceanRadiusGen[2], std::mutex mtx);
    // Multithreading function to create the land
    void thrNoise(double **landGrid, int xOffset, int yOffset, int seed, std::mutex mtx);
    float warpedNoise(float x, float y, float freq, int depth, int seed);
};

/*  Since grid squares contain large amounts of data, a struct is best to represent each square
*   These should only be used within the grid
*/
struct gridSquare
{
    public:
    // Owner ID
    uint8_t owner;
    // Terrain type
    uint8_t terrain;
    // Temporary until unit class gets added
    uint8_t unit;
    // Facilities of the grid square
    uint8_t facilities;
    // Population of the square
    unsigned int population;
    // Tile height, used to generate some things.
    double height;
};

/*  Rendered form of the grid, the COMPLETE GRID
*   Used primarily to generate the viewable grid
*/
class renderedGrid
{
    public:
    // Actual content of the grid, initialized to 2d array of varying size
    char **gridContent;
};

class viewGrid 
{
    public:
    /*  Actual content of the grid
    *   The view grid is 80x80, however the array is 80x81 to account for the null character
    *   Initialize each pointer to a char array with 81 length
    */
    char **gridContent;
    void generate(int xOffset, int yOffset, renderedGrid grid);
};

#endif