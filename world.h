#ifndef WORLD
#define WORLD
#include "data_structures.h"

#define LARGE_NATION_SIZE 5
#define LARGE_NATION_COUNT 5

#define MEDIUM_NATION_SIZE 3
#define MEDIUM_NATION_COUNT 60

#define SMALL_NATION_SIZE 1
#define SMALL_NATION_COUNT 120

#define NATION_COUNT LARGE_NATION_COUNT + MEDIUM_NATION_COUNT + SMALL_NATION_COUNT
#define STATE_COUNT LARGE_NATION_COUNT * LARGE_NATION_SIZE + MEDIUM_NATION_COUNT * MEDIUM_NATION_SIZE + SMALL_NATION_COUNT * SMALL_NATION_SIZE

#define AGRICULTURE_PER_STATE 2
#define FOSSIL_FUEL_CHANCE 0.05
#define RAW_MATERIALS_PER_STATE 1
#define PRECIOUS_STONES_CHANCE 0.01
#define INDUSTRIES_PER_STATE 2

Nation gNations[NATION_COUNT];
State gStates[STATE_COUNT];

void init_nations();

#endif