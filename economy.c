#include "constants.h"
#include "data_structures.h"
#include "economy.h"

void run_national_production(Nation *nation) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        int resourceBalance = 0;
        for (int j = 0; j < nation->stateCount; j++) {
            int stateProduction = nation->states[j].resources[i].production;
            int stateConsumption = nation->states[j].resources[i].consumption;
            resourceBalance += stateProduction - stateConsumption;
        }

        if (resourceBalance > 0) {
            nation->exports[i].amount = resourceBalance;
            nation->imports[i].amount = 0;
        }
        else if (resourceBalance < 0) {
            nation->exports[i].amount = 0;
            nation->imports[i].amount = - resourceBalance;
        }
        else {
            nation->exports[i].amount = 0;
            nation->imports[i].amount = 0;
        }
    }
}

void run_market(Market *market) {
}