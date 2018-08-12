#include "constants.h"
#include "data_structures.h"
#include "economy.h"

int produce_resource(Nation *nation, State *state, int type);

void calculate_resource_consumption(Nation *nation) {}

void run_national_production(Nation *nation) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        int resourceBalance = 0;
        int production = 0;
        int consumption = 0;
        for (int j = 0; j < nation->stateCount; j++) {
            int stateProduction = produce_resource(nation, &nation->states[j], i);
            int stateConsumption = nation->states[j].resources[i].consumption;
            production += stateProduction;
            consumption += stateConsumption;
            resourceBalance += stateProduction - stateConsumption;
        }

        nation->resources[i].production = production;
        nation->resources[i].consumption = consumption;
        if (resourceBalance > 0) {
            nation->exports[i].surplus = resourceBalance;
            nation->imports[i].deficit = 0;
        }
        else if (resourceBalance < 0) {
            nation->exports[i].surplus = 0;
            nation->imports[i].deficit = - resourceBalance;
        }
        else {
            nation->exports[i].surplus = 0;
            nation->imports[i].deficit = 0;
        }
    }
}

void run_market(Market *market) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        ResourceMarket rMarket = market->markets[i];
        int exports = 0;
        int imports = 0;
        for (int j = 0; j < rMarket.exportCount; j++) {
            exports += rMarket.exports[j]->surplus - rMarket.exports[j]->exported;
        }

        for (int j = 0; j < rMarket.importCount; j++) {
            imports += rMarket.imports[j]->deficit - rMarket.imports[j]->imported;
        }

        float importSatisfaction = (float)exports / imports;
        /*Imports are filled, exports are rationed*/
        if (importSatisfaction >= 1.0f) {
            for (int j = 0; j < rMarket.importCount; j++) {
                rMarket.imports[j]->imported = rMarket.imports[j]->deficit;
            }

            float exportSatisfaction = (float)imports / exports;
            for (int j = 0; j < rMarket.exportCount; j++) {
                int surplus = rMarket.exports[j]->surplus - rMarket.exports[j]->exported;
                rMarket.exports[j]->exported += (int)(surplus * exportSatisfaction);
            }
        }
        /*Exports are filled, imports are rationed*/
        else {
            for (int j = 0; j < rMarket.exportCount; j++) {
                rMarket.exports[j]->exported = rMarket.exports[j]->surplus;
            }

            for (int j = 0; j < rMarket.importCount; j++) {
                int deficit = rMarket.imports[j]->deficit - rMarket.imports[j]->imported;
                rMarket.imports[j]->imported += (int)(deficit * importSatisfaction);
            }
        }
    }
}

void calculate_resource_satisfaction(Nation *nation) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        int resourceSupply = nation->resources[i].production + nation->imports[i].imported;
        int resourceDemand = nation->resources[i].consumption;
        if (resourceSupply < resourceDemand) {
            nation->resourceSatisfaction[i] = (float)resourceSupply / resourceDemand;
        }
        else {
            nation->resourceSatisfaction[i] = 1.0f;
        }
    }
}

void increase_production(State *state) {}

/*Production of a resource is proportional to the average resource satisfactions of any dependent resources*/
int produce_resource(Nation *nation, State *state, int type) {
    int production = state->resources[type].production;
    switch (type)
    {
        case MEAT:
            return (int)(production * nation->resourceSatisfaction[CEREALS]);
        case DAIRY:
            return (int)(production * nation->resourceSatisfaction[CEREALS]);
        case FOSSIL_FUELS:
            return (int)(production * nation->resourceSatisfaction[MACHINERY]);
        case WOOD_AND_PAPER:
            return (int)(production * ((
                nation->resourceSatisfaction[MACHINERY] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2));
        case MINERALS:
            return (int)(production * nation->resourceSatisfaction[MACHINERY]);
        case IRON_AND_STEEL:
            return (int)(production * nation->resourceSatisfaction[ELECTRICITY]);
        case PRECIOUS_STONES:
            return (int)(production * nation->resourceSatisfaction[MACHINERY]);
        case FABRICS:
            return (int)(production * nation->resourceSatisfaction[ELECTRICITY]);
        case PLASTICS:
            return (int)(production * ((
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2));
        case CHEMICALS:
            return (int)(production * nation->resourceSatisfaction[ELECTRICITY]);
        case PHARMACEUTICALS:
            return (int)(production * ((
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2));
        case APPLIANCES:
            return (int)(production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2));
        case VEHICLES:
            return (int)(production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[PLASTICS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 3));
        case MACHINERY:
            return (int)(production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2));
        case COMMODITIES:
            return (int)(production * ((
                nation->resourceSatisfaction[PLASTICS] +
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[FABRICS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 4));
        case CONSTRUCTION:
            return (int)(production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[WOOD_AND_PAPER] +
                nation->resourceSatisfaction[MACHINERY]
            ) / 3));
        case HEALTHCARE:
            return (int)(production * nation->resourceSatisfaction[PHARMACEUTICALS]);
        case RETAIL:
            return (int)(production * nation->resourceSatisfaction[MARKETING]);
        default:
            return production;
    }
}