#include "stdlib.h"
#include "world.h"
#include "utils.h"
#define currentNation gNations[gNationsIndex]
#define currentState gStates[gStatesIndex]

int gNationsIndex = 0;
int gStatesIndex = 0;
void init_nation_group(int nationCount, int stateCount);
void init_resources();

void init_nations() {
    init_nation_group(LARGE_NATION_COUNT, LARGE_NATION_SIZE);
    init_nation_group(MEDIUM_NATION_COUNT, MEDIUM_NATION_SIZE);
    init_nation_group(SMALL_NATION_COUNT, SMALL_NATION_SIZE);
}

void init_nation_group(int nationCount, int stateCount) {
    for (int i = 0; i < nationCount; i++) {
        /*Initialise states*/
        currentNation.states = &currentState;
        currentNation.stateCount = stateCount;
        for (int j = 0; j < stateCount; j++) {
            currentNation.states[j].controllingNation = &currentNation;
            init_resources();
            currentState.population = select_range(STATE_POP_MIN, STATE_POP_MAX);
            gStatesIndex++;
        }

        /*Initialise nation imports/exports*/
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            currentNation.exports[j].exportingNation = &currentNation;
            currentNation.imports[j].importingNation = &currentNation;
        }

        gNationsIndex++;
    }
}

void init_resources() {
    /*Agriculture*/
    for (int i = 0; i < AGRICULTURE_PER_STATE; i++) {
        int agricultureSelection = select_value(
            6,
            (int[]) { CEREALS, VEGETABLES_AND_FRUIT, MEAT, DAIRY, TOBACCO, DRUGS }
        );
        currentState.resources[agricultureSelection].production +=
            select_range(AGRICULTRE_START_MIN, AGRICULTURE_START_MAX);
    }

    /*Fossil fuels*/
    if (FOSSIL_FUEL_CHECK) {
        currentState.resources[FOSSIL_FUELS].production +=
            select_range(FOSSIL_FUEL_START_MIN, FOSSIL_FUEL_START_MAX);
    }

    /*Raw materials*/
    for (int i = 0; i < RAW_MATERIALS_PER_STATE; i++) {
        int rawMaterialSelection = select_value(
            3,
            (int[]) { WOOD_AND_PAPER, MINERALS, IRON_AND_STEEL }
        );
        currentState.resources[rawMaterialSelection].production +=
            select_range(RAW_MATERIAL_START_MIN, RAW_MATERIAL_START_MAX);
    }

    /*Precious stones*/
    if (PRECIOUS_STONES_CHECK) {
        currentState.resources[PRECIOUS_STONES].production +=
            select_range(PRECIOUS_STONES_MIN, PRECIOUS_STONES_MAX);
    }
}