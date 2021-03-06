#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES
#include "constants.h"

typedef struct Resource Resource;
typedef struct State State;
typedef struct Military Military;
typedef struct GovernmentSpending GovernmentSpending;
typedef struct Government Government;
typedef struct Nation Nation;
typedef struct ResourceMarket ResourceMarket;
typedef struct Market Market;
typedef struct Export Export;
typedef struct Import Import;

struct Resource
{
    float production;
    float consumption;
};

struct Export
{
    Nation *exportingNation;
    /*The surplus that occurs when only national consumption is available
    this number is unaffected by the exported amount*/
    float surplus;
    float exported;
};

struct Import
{
    Nation *importingNation;
    /*The shortage that occurs when only national production is available,
    this number is unaffected by the imported amount*/
    float deficit;
    float imported;
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
    /*bitfield*/
    int producedResources;
    Nation *controllingNation;
    Nation *coreNation;
    float population;
};

struct Military
{
    int infantry;
    int artillery;
    int armoured;
    int airFighters;
    int airBombers;
    /*Surface to air missiles*/
    int sams;
};

struct GovernmentSpending
{
    float base;
    float modifier;
    float current;
};

struct Government
{
    int governmentType;
    Military military;
    GovernmentSpending spending[GOVERNMENT_SPENDING_AREAS];
    float taxRate;
    float revenue;
    float totalSpending;
    float treasury;
    float foreignRelations[NATION_COUNT];
};

struct Nation
{
    State *states;
    int stateCount;
    Export exports[RESOURCE_COUNT];
    Import imports[RESOURCE_COUNT];
    Government government;
    /*Ratio of produced to consumed of the resource (maxed at 1.0)*/
    float resourceSatisfaction[RESOURCE_COUNT];
    /*How much consumption remains unfulfilled*/
    float resourceShortage[RESOURCE_COUNT];
    float stability;
    float hdi;
    float happiness;
    float corruption;
    /*Computed from stability and hdi*/
    float economicGrowth;
    /*Computed from states*/
    Resource resources[RESOURCE_COUNT];
    float population;
    /*Computed from states gdp*/
    float gdp;
    /*Computed from gdp and population*/
    float gdpPerCapita;
};

#endif