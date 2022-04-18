#include <cstdint>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
#include "grid.hpp"
#include "civ_typedefs.hpp"
#include "checks.hpp"
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
        double **oceanGrid = (double**)malloc(sizeof(double*) * ySize);
        for(int i = 0; i < ySize; i++)
        {
            oceanGrid[i] = (double*)malloc(sizeof(double) * xSize);
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
                oceanRadiusGen[0] = SUPER_SMALL_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SUPER_SMALL_OCEANS_RADIUS_MIN;
                break;
            case SMALL:
                oceanCount = randInRange(SMALL_OCEANS_MAX, SMALL_OCEANS_MIN);
                oceanRatio = randInRange(SMALL_OCEANS_RATIO_MAX, SMALL_OCEANS_RATIO_MIN);
                oceanRadiusGen[0] = SMALL_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SMALL_OCEANS_RADIUS_MIN;
                break;
            case MEDIUM:
                oceanCount = randInRange(MEDIUM_OCEANS_MAX, MEDIUM_OCEANS_MIN);
                oceanRatio = randInRange(MEDIUM_OCEANS_RATIO_MAX, MEDIUM_OCEANS_RATIO_MIN);
                oceanRadiusGen[0] = MEDIUM_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = MEDIUM_OCEANS_RADIUS_MIN;
                break;
            case LARGE:
                oceanCount = randInRange(LARGE_OCEANS_MAX, LARGE_OCEANS_MIN);
                oceanRatio = randInRange(LARGE_OCEANS_RATIO_MAX, LARGE_OCEANS_RATIO_MIN);
                oceanRadiusGen[0] = LARGE_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = LARGE_OCEANS_RADIUS_MIN;
                break;
            case SUPER_LARGE:
                oceanCount = randInRange(SUPER_LARGE_OCEANS_MAX, SUPER_LARGE_OCEANS_MIN);
                oceanRatio = randInRange(SUPER_LARGE_OCEANS_RATIO_MAX, SUPER_LARGE_OCEANS_RATIO_MIN);
                oceanRadiusGen[0] = SUPER_LARGE_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SUPER_LARGE_OCEANS_RADIUS_MIN;
                break;
            default:
                std::stringstream err;
                err << "Invaild world size! World size should be one of the valid sizes defined in grid.hpp, not " << xSize << "!";
                throw err.str();
        }
        std::mutex mtx;
        std::vector<std::thread> threads;
        for(int i = 0; i < oceanCount; i++)
        {
            threads.push_back(std::thread(&thrOceans, oceanGrid, oceanRatio, oceanRadiusGen, mtx));
        }
        for(int i = 0; i < oceanCount; i++)
        {
            threads.at(i).join();
        }
        threads.clear();
        int seed = randInRange(randInRange(SUPER_LARGE_OCEANS_MAX));
        double **landGrid = (double**)malloc(sizeof(double*) * ySize);
        for(int i = 0; i < xSize; i++)
        {
            landGrid[i] = (double*)malloc(sizeof(double) * xSize);
        }
        for(int i = 0; i < ySize/GRID_PROCESSING_CHUNK_SIZE; i++)
        {
            for(int j = 0; j < xSize/GRID_PROCESSING_CHUNK_SIZE; j++)
            {
                threads.push_back(std::thread(&thrNoise, landGrid, GRID_PROCESSING_CHUNK_SIZE*j, GRID_PROCESSING_CHUNK_SIZE*i, seed, mtx));
            }
        }
        for(int i = 0; i < ySize/GRID_PROCESSING_CHUNK_SIZE; i++)
        {
            for(int j = 0; j < xSize/GRID_PROCESSING_CHUNK_SIZE; j++)
            {
                threads.front().join();
            }
        }
        for(int i = 0; i < ySize; i++)
        {
            for(int j = 0; j < xSize; j++)
            {
                landGrid[i][j] -= oceanGrid[i][j];
                squares[i][j].height = landGrid[i][j];
            }
        }
    }
    // Multithreading function to generate the oceans.
    void thrOceans(double **oceanGrid, int oceanRatio, int oceanRadiusGen[2], std::mutex mtx)
    {
        std::lock_guard<std::mutex> lock (mtx);
        int2 center;
        center.x = randInRange(xSize - 1);
        center.y = randInRange(ySize - 1);
        int2 origin = center;
        for(int i = 0; i < oceanRatio; i++)
        {
            int2 pos;
            pos.x = randInRange(xSize - 1);
            pos.y = randInRange(ySize - 1);
            while(!inCircle(pos, origin, oceanRadiusGen[0] * 1 + sin((double)i)))
            {
                pos.x = randInRange(xSize - 1);
                pos.y = randInRange(ySize - 1);
            }
            int radius = randInRange(oceanRadiusGen[0], oceanRadiusGen[1]);
            oceanGrid[pos.y][pos.x] = 1;
            for(int j = 0; j < radius; j++)
            {
                for(int k = 0; k < radius; k++)
                {
                    int2 tmp;
                    tmp.x = j;
                    tmp.y = k;
                    if(inCircle(tmp, pos, radius - 3))
                    {
                        oceanGrid[k][j] = 1;
                    }
                    else if(inCircle(tmp, pos, radius - 1))
                    {
                        oceanGrid[k][j] = .7;
                    }
                    else if(inCircle(tmp, pos, radius))
                    {
                        oceanGrid[k][j] = .5;
                    }
                }
            }
            origin = pos;
        }
    }
    // Multithreading function to create the land
    void thrNoise(double **landGrid, int xOffset, int yOffset, int seed, std::mutex mtx)
    {
        std::lock_guard<std::mutex> lock (mtx);
        for(int i = yOffset; i < GRID_PROCESSING_CHUNK_SIZE + yOffset; i++)
        {
            for(int j = xOffset; j < GRID_PROCESSING_CHUNK_SIZE + xOffset; j++)
            {
                landGrid[i][j] = (double)warpedNoise((float)j, (float)i, PERLIN_NOISE_FREQUENCY, PERLIN_NOISE_DEPTH, seed);
            }
        }
    }
    float warpedNoise(float x, float y, float freq, int depth, int seed)
    {
        return perlin2d(x * perlin2d(x, y, freq, depth, seed), y * perlin2d(x, y, freq, depth, seed), freq * perlin2d(x, y, freq, depth, seed), (int)((float)depth * perlin2d(x, y, freq, depth, seed)), (int)((float)seed * perlin2d(x, y, freq, depth, seed)));
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
    // Tile height, used to generate some things.
    double height;
    gridSquare()
    {
        owner = NULL;
        terrain = NULL;
        unit = NULL;
        facilities = NULL;
        population = NULL;
        height = NULL;
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