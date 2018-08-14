#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES
#include "constants.h"

typedef struct Resource Resource;
typedef struct State State;
typedef struct Nation Nation;
typedef struct ResourceMarket ResourceMarket;
typedef struct Market Market;
typedef struct Export Export;
typedef struct Import Import;

struct Resource
{
    int production;
    int consumption;
};

struct Export
{
    Nation *exportingNation;
    /*The surplus that occurs when only national consumption is available
    this number is unaffected by the exported amount*/
    int surplus;
    int exported;
};

struct Import
{
    Nation *importingNation;
    /*The shortage that occurs when only national production is available,
    this number is unaffected by the imported amount*/
    int deficit;
    int imported;
};

/*imports and exports are an array of pointers*/
struct ResourceMarket
{
    int exportCount;
    Export **exports;
    int importCount;
    Import **imports;
};

struct Market
{
    ResourceMarket markets[RESOURCE_COUNT];
};

struct State
{
    Resource resources[RESOURCE_COUNT];
    Nation *controllingNation;
    int population;
};

struct Nation
{
    State *states;
    int stateCount;
    Export exports[RESOURCE_COUNT];
    Import imports[RESOURCE_COUNT];
    float resourceSatisfaction[RESOURCE_COUNT];
    float stability;
    float hdi;
    /*Computed from stability and hdi*/
    float economicGrowth;
    /*Computed from states*/
    Resource resources[RESOURCE_COUNT];
    int population;
    /*Computed from states production*/
    int gdp;
    /*Computed from gdp and population*/
    int gdpPerCapita;
};

#endif