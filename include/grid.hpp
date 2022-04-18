#ifndef CIVILIZATION_GRID_HPP
#define CIVILIZATION_GRID_HPP

#include <cstdint>

#ifndef VIEWGRID_SIZE
#define VIEWGRID_SIZE 80
#endif

#ifndef WORLD_SIZES
#define WORLD_SIZES 0x5
#define SUPER_SMALL 0x50
#define SMALL 0xA0
#define MEDIUM 0xF0
#define LARGE 0x190
#define SUPER_LARGE 0x230
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