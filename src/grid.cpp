#include <cstdint>
#include "grid.hpp"
#include "perlin.hpp"
#include "random.hpp"

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
    Grid()
    {
        throw "Invalid constructor! Use the other constructor to intiate the grid!";
    }
    // Actual constructor
    Grid(int size)
    {
        xSize = size;
        ySize = size;
        squares = (gridSquare**)malloc(sizeof(gridSquare*) * size);
        for(int i = 0; i < size; i++)
        {
            squares[i] = (gridSquare*)malloc(sizeof(gridSquare) * size);
            for(int j = 0; j < size; j++)
            {
                squares[i][j] = gridSquare();
            }
        }
        generateWorld();
    }
    // Self explanitory, renders the grid
    renderedGrid render();
    private:
    // Generates the world
    void generateWorld()
    {
        int **oceanGrid = (int**)malloc(sizeof(int*) * ySize);
        for(int i = 0; i < ySize; i++)
        {
            oceanGrid[i] = (int*)malloc(sizeof(int) * xSize);
        }
        int yOceanLimit = randInRange(ySize/20);
        for(int i = 0; i < ySize; i++)
        {
            for(int j = 0; j < xSize; j++)
            {
                if(i > yOceanLimit && i < ySize - yOceanLimit - 1)
                {
                    if(j < 1 || j > ySize - 2)
                    {
                        oceanGrid[i][j] = 1;
                    }
                }
            }
        }
        int oceanCount;
        int oceanRatio;
        int oceanRadiusGen[2];
        switch(xSize)
        {
            case SUPER_SMALL:
                oceanCount = randInRange(SUPER_SMALL_OCEANS_MAX, SUPER_SMALL_OCEANS_MIN);
                oceanRatio = randInRange(SUPER_SMALL_OCEANS_RATIO_MAX, SUPER_SMALL_OCEANS_RATIO_MIN);
                oceanRadiusGen = { SUPER_SMALL_OCEANS_RADIUS_MAX, SUPER_SMALL_OCEANS_RADIUS_MIN };
                break;
            case SMALL:
                oceanCount = randInRange(SMALL_OCEANS_MAX, SMALL_OCEANS_MIN);
                oceanRatio = randInRange(SMALL_OCEANS_RATIO_MAX, SMALL_OCEANS_RATIO_MIN);
                oceanRadiusGen = { SMALL_OCEANS_RADIUS_MAX, SMALL_OCEANS_RADIUS_MIN };
                break;
            case MEDIUM:
                oceanCount = randInRange(MEDIUM_OCEANS_MAX, MEDIUM_OCEANS_MIN);
                oceanRatio = randInRange(MEDIUM_OCEANS_RATIO_MAX, MEDIUM_OCEANS_RATIO_MIN);
                oceanRadiusGen = { MEDIUM_OCEANS_RADIUS_MAX, MEDIUM_OCEANS_RADIUS_MIN };
                break;
            case LARGE:
                oceanCount = randInRange(LARGE_OCEANS_MAX, LARGE_OCEANS_MIN);
                oceanRatio = randInRange(LARGE_OCEANS_RATIO_MAX, LARGE_OCEANS_RATIO_MIN);
                oceanRadiusGen = { LARGE_OCEANS_RADIUS_MAX, LARGE_OCEANS_RADIUS_MIN };
                break;
            case SUPER_LARGE:
                oceanCount = randInRange(SUPER_LARGE_OCEANS_MAX, SUPER_LARGE_OCEANS_MIN);
                oceanRatio = randInRange(SUPER_LARGE_OCEANS_RATIO_MAX, SUPER_LARGE_OCEANS_RATIO_MIN);
                oceanRadiusGen = { SUPER_LARGE_OCEANS_RADIUS_MAX, SUPER_LARGE_OCEANS_RADIUS_MIN };
                break;
        }
    }
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
    gridSquare()
    {
        owner = NULL;
        terrain = NULL;
        unit = NULL;
        facilities = NULL;
        population = NULL;
    }
};

/*  Rendered form of the grid, the COMPLETE GRID
*   Used primarily to generate the viewable grid
*/
class renderedGrid
{
    public:
    // Actual content of the grid, initialized to 2d array of varying size
    char *gridContent;
};

class viewGrid 
{
    public:
    /*  Actual content of the grid
    *   The view grid is 80x80, however the array is 80x81 to account for the null character
    *   Initialize each pointer to a char array with 81 length
    */
    char *gridContent;
    void generate(int xOffset, int yOffset, renderedGrid grid);
};