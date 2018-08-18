#include "stdlib.h"
#include "demographics.h"
#include "economy.h"
#include "utils.h"
#include "world.h"
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

        calculate_population(&currentNation);
        gNationsIndex++;
    }
}

void init_resources() {
    /*Cereal*/
    enable_resource_production(&currentState, CEREALS);
    currentState.resources[CEREALS].production +=
        (float)select_range(CEREAL_START_MIN, CEREAL_START_MAX);
    currentNation.gdp += currentState.resources[CEREALS].production;

    /*Agriculture*/
    for (int i = 0; i < AGRICULTURE_PER_STATE; i++) {
        int agricultureSelection = select_value(
            5,
            (int[]) { VEGETABLES_AND_FRUIT, MEAT, DAIRY, TOBACCO, DRUGS }
        );
        enable_resource_production(&currentState, agricultureSelection);
    }

    /*Electricity*/
    enable_resource_production(&currentState, ELECTRICITY);
    /*Fossil fuels*/
    if (FOSSIL_FUEL_CHECK) {
        enable_resource_production(&currentState, FOSSIL_FUELS);
    }

    /*Raw materials*/
    for (int i = 0; i < RAW_MATERIALS_PER_STATE; i++) {
        int rawMaterialSelection = select_value(
            3,
            (int[]) { WOOD_AND_PAPER, MINERALS, IRON_AND_STEEL }
        );
        enable_resource_production(&currentState, rawMaterialSelection);
    }

    /*Precious stones*/
    if (PRECIOUS_STONES_CHECK) {
        enable_resource_production(&currentState, PRECIOUS_STONES);
    }

    /*Industry*/
    for (int i = 0; i < INDUSTRY_PER_STATE; i++) {
        int industrySelection = select_value(
            4,
            (int[]) { FABRICS, PLASTICS, CHEMICALS, PHARMACEUTICALS }
        );
        enable_resource_production(&currentState, industrySelection);
    }

    /*Finished goods*/
    for (int i = 0; i < FINISHED_GOODS_PER_STATE; i++) {
        int finishedGoodsSelection = select_value(
            4,
            (int[]) { APPLIANCES, MACHINERY, VEHICLES, COMMODITIES }
        );
        enable_resource_production(&currentState, finishedGoodsSelection);
    }

    /*Services*/
    enable_resource_production(&currentState, CONSTRUCTION);
    enable_resource_production(&currentState, ENGINEERING);
    enable_resource_production(&currentState, HEALTHCARE);
    enable_resource_production(&currentState, RETAIL);
    enable_resource_production(&currentState, LEGAL_SERVICES);
    enable_resource_production(&currentState, MARKETING);
}