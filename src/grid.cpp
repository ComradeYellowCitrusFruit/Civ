#include <cstdint>
#include <thread>
#include <mutex>
#include <sstream>
#include <vector>
#include "grid.hpp"
#include "gamestate.hpp"
#include "civ_typedefs.hpp"
#include "countries.hpp"
#include "units.hpp"
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
        int countries;
        switch(xSize)
        {
            case SUPER_SMALL:
                oceanCount = randInRange(SUPER_SMALL_OCEANS_MAX, SUPER_SMALL_OCEANS_MIN);
                oceanRatio = randInRange(SUPER_SMALL_OCEANS_RATIO_MAX, SUPER_SMALL_OCEANS_RATIO_MIN);
                countries = randInRange(SUPER_SMALL_COUNTRIES_MIN, SUPER_SMALL_COUNTRIES_MAX);
                oceanRadiusGen[0] = SUPER_SMALL_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SUPER_SMALL_OCEANS_RADIUS_MIN;
                break;
            case SMALL:
                oceanCount = randInRange(SMALL_OCEANS_MAX, SMALL_OCEANS_MIN);
                oceanRatio = randInRange(SMALL_OCEANS_RATIO_MAX, SMALL_OCEANS_RATIO_MIN);
                countries = randInRange(SMALL_COUNTRIES_MIN, SMALL_COUNTRIES_MAX);
                oceanRadiusGen[0] = SMALL_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SMALL_OCEANS_RADIUS_MIN;
                break;
            case MEDIUM:
                oceanCount = randInRange(MEDIUM_OCEANS_MAX, MEDIUM_OCEANS_MIN);
                oceanRatio = randInRange(MEDIUM_OCEANS_RATIO_MAX, MEDIUM_OCEANS_RATIO_MIN);
                countries = randInRange(MEDIUM_COUNTRIES_MIN, MEDIUM_COUNTRIES_MAX);
                oceanRadiusGen[0] = MEDIUM_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = MEDIUM_OCEANS_RADIUS_MIN;
                break;
            case LARGE:
                oceanCount = randInRange(LARGE_OCEANS_MAX, LARGE_OCEANS_MIN);
                oceanRatio = randInRange(LARGE_OCEANS_RATIO_MAX, LARGE_OCEANS_RATIO_MIN);
                countries = randInRange(LARGE_COUNTRIES_MIN, LARGE_COUNTRIES_MAX);
                oceanRadiusGen[0] = LARGE_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = LARGE_OCEANS_RADIUS_MIN;
                break;
            case SUPER_LARGE:
                oceanCount = randInRange(SUPER_LARGE_OCEANS_MAX, SUPER_LARGE_OCEANS_MIN);
                oceanRatio = randInRange(SUPER_LARGE_OCEANS_RATIO_MAX, SUPER_LARGE_OCEANS_RATIO_MIN);
                countries = randInRange(SUPER_LARGE_COUNTRIES_MIN, SUPER_LARGE_COUNTRIES_MAX);
                oceanRadiusGen[0] = SUPER_LARGE_OCEANS_RADIUS_MAX;
                oceanRadiusGen[1] = SUPER_LARGE_OCEANS_RADIUS_MIN;
                break;
            default:
                std::stringstream err;
                err << "Invaild world size! World size should be one of the valid sizes defined in grid.hpp, not " << xSize << "!";
                throw err.str().c_str();
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
                squares[i][j].height = landGrid[i][j] * (fabs((double)perlin2d((float)j, (float)i, randInRange(120), randInRange(120), seed))*10);
            }
        }
        for(int i = 0; i < ySize; i++)
        {
            free(oceanGrid[i]);
            free(landGrid[i]);
        }
        free(oceanGrid);
        free(landGrid);
        int2 **biomes = (int2**)malloc(sizeof(int2*) * 7);
        for(int i = 0; i < 8; i++)
        {
            biomes[i] = (int2*)malloc(sizeof(int2) * randInRange(24, 2));
        }
        int southLimit = ySize/randInRange(24,4);
        int northLimit = ySize - (ySize/randInRange(24,4));
        for(int i = 0; i < 7; i++)
        {
            for(int j = 0; j < sizeof(biomes[i])/sizeof(int2); j++)
            {
                biomes[i][j].y = randInRange(northLimit, southLimit);
                biomes[i][j].x = randInRange(xSize);
                while(squares[biomes[i][j].y][biomes[i][j].x].height <= 3)
                {
                    biomes[i][j].y = randInRange(northLimit, southLimit);
                    biomes[i][j].x = randInRange(xSize);
                }
            }
        }
        threads.clear();
        uint8_t biomeTypes[7] = { terrain::plains, terrain::hills, terrain::forest, terrain::swamp, terrain::mountain, terrain::tundra };
        for(int i = 0; i < 7; i++)
        {
            threads.push_back(std::thread(&thrTerrain, biomes[i], northLimit, southLimit, oceanRadiusGen[1]/2, oceanRadiusGen[0]/2, biomeTypes[i], mtx));
        }
        for(int i = 0; i < 7; i++)
        {
            threads.at(i).join();
        }
        threads.clear();
        bool firstRun = true;
        for(int i = 0; i <= countries; i++)
        {
            int2 pos;
            pos.x = randInRange(xSize);
            pos.y = randInRange(northLimit, southLimit);
            while(!squares[pos.y][pos.x].validLocation())
            {
                pos.x = randInRange(xSize);
                pos.y = randInRange(northLimit, southLimit);
            }
            country Country;
            switch(firstRun)
            {
                case true:
                    firstRun = false;
                    Country.id = PLAYER_RESERVED_ID;
                    break;
                case false:
                    Country.id = COUNTY_ID_START_POINT + (i - 1);
                    break;
            }
            unit settler;
            settler = Settler;
            settler.owner = Country.id;
            settler.setID(idTracker + 1);
            idTracker++;
            settler.position = pos;
            settler.canMove = true;
            Main.units.push_back(settler);
            unit warrior;
            warrior = Warrior;
            warrior.owner = Country.id;
            warrior.setID(idTracker + 1);
            idTracker++;
            bool validPos = false;
            int2 Pos = pos;
            while(!validPos)
            {
                if(squares[pos.y + 1][pos.x].validLocation())
                {
                    Pos.y++;
                }
                else if(squares[pos.y][pos.x + 1].validLocation())
                {
                    Pos.x++;
                }
                else if(squares[pos.y + 1][pos.x + 1].validLocation())
                {
                    Pos.y++;
                    Pos.x++;
                }
                else if(squares[pos.y - 1][pos.x].validLocation())
                {
                    Pos.y--;
                }
                else if(squares[pos.y][pos.x - 1].validLocation())
                {
                    Pos.x--;
                }
                else if(squares[pos.y - 1][pos.x - 1].validLocation())
                {
                    Pos.y--;
                    Pos.x--;
                }
                else if(squares[pos.y + 1][pos.x - 1].validLocation())
                {
                    Pos.y++;
                    Pos.x--;
                }
                else if(squares[pos.y - 1][pos.x + 1].validLocation())
                {
                    Pos.y--;
                    Pos.x++;
                }
            }
            warrior.canMove = true;
            Main.units.push_back(warrior);
            Main.countries.push_back(Country);
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
                landGrid[i][j] = (double)warpedNoise((float)j, (float)i, PERLIN_NOISE_FREQUENCY, PERLIN_NOISE_DEPTH, seed) - ((double)warpedNoise((float)j, (float)i, randInRange(120), randInRange(120), seed) * .25);
            }
        }
    }
    // Multithreading function to process terrain
    void thrTerrain(int2 *biomes, int northLimit, int southLimit, int rmin, int rmax, uint8_t flag, std::mutex mtx)
    {
        for(int i = 0; i < ySize; i++)
        {
            for(int j = 0; j < xSize; j++)
            {
                if(i > northLimit || i < southLimit)
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    if(squares[i][j].height >= 3 && squares[i][j].terrain & terrain::ice != terrain::ice)
                    {
                        if(squares[i][j].height == 3 || squares[i][j].height <= 4)
                        {
                            squares[i][j].height == randDoubleInRange(6, 4);
                        }
                        squares[i][j].terrain |= terrain::ice;
                    }
                }
                else
                {
                    for(int k = 0; k < 8; k++)
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        if(searchPoints(&(biomes[k]), i, j, 8))
                        {
                            int radius = randInRange(rmax, rmin);
                            for(int y = 0; y < ySize; y++)
                            {
                                for(int x = 0; x < xSize; x++)
                                {
                                    int2 point;
                                    point.x = x;
                                    point.y = y;
                                    if(inCircle(point, searchPoints(biomes[k], i, j), radius))
                                    {
                                        squares[y][x].terrain |= flag;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    int2 searchPoints(int2 *points, int x, int y)
    {
        int2 target;
        target.x = x;
        target.y = y;
        for(int i = 0; i < sizeof(points[i])/sizeof(int2); i++)
        {
            switch(points[i])
            {
                case target:
                    return points[i];
                default:
                    break;
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
    int owner;
    // Terrain type
    uint8_t terrain;
    // Temporary until unit class gets added
    uint8_t unit;
    // Facilities of the grid square
    uint32_t facilities;
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
    bool validLocation()
    {
        if(height <= 3)
        {
            return false;
        }
        switch(terrain & terrain::ice & terrain::mountain)
        {
            case terrain::ice:
                return false;
            case terrain::mountain:
                return false;
            case terrain::ice | terrain::mountain:
                return false;
            case (uint8_t)0x0:
                return true;
        }
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

enum terrain
{
    ice = 1 << 7,
    tundra = 1 << 6,
    mountain = 1 << 5,
    desert = 1 << 4,
    swamp = 1 << 3,
    forest = 1 << 2,
    hills = 1 << 1,
    plains = 1 << 0
};