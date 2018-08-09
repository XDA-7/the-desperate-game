#include "stdlib.h"
#include "world.h"
/*Use a macro for current nation/state?*/

int gNationsIndex = 0;
int gStatesIndex = 0;
void init_nation_group(int nationCount, int stateCount);

void init_nations() {
    init_nation_group(LARGE_NATION_COUNT, LARGE_NATION_SIZE);
    init_nation_group(MEDIUM_NATION_COUNT, MEDIUM_NATION_SIZE);
    init_nation_group(SMALL_NATION_COUNT, SMALL_NATION_SIZE);
}

void init_nation_group(int nationCount, int stateCount) {
    for (int i = 0; i < nationCount; i++) {
        /*Initialise states*/
        gNations[gNationsIndex].states = &gStates[gStatesIndex];
        gNations[gNationsIndex].stateCount = stateCount;
        for (int j = 0; j < stateCount; j++) {
            gNations[gNationsIndex].states[j].controllingNation = &gNations[gNationsIndex];
            /*Initialise resources*/
        }

        /*Initialise nation imports/exports*/
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            gNations[gNationsIndex].exports[j].exportingNation = &gNations[gNationsIndex];
            gNations[gNationsIndex].imports[j].importingNation = &gNations[gNationsIndex];
        }

        gNationsIndex++;
        gStatesIndex += stateCount;
    }
}