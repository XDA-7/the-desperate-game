#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES
#include "constants.h"

typedef struct Resource Resource;
typedef struct State State;
typedef struct Nation Nation;
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
    int amount;
};

struct Import
{
    Nation *importingNation;
    int amount;
};

struct Market
{
    Export *exports[RESOURCE_COUNT];
    Import *imports[RESOURCE_COUNT];
};

struct State
{
    Resource resources[RESOURCE_COUNT];
    Nation *controllingNation;
};

struct Nation
{
    State *states;
    Resource resources[RESOURCE_COUNT];
    Export exports[RESOURCE_COUNT];
    Import imports[RESOURCE_COUNT];
};

#endif