#include "constants.h"
#include "data_structures.h"
#include "economy.h"

int produce_resource(Nation *nation, State *state, int type);
void calculate_industry_consumptions(Nation *nation);
void calculate_industry_consumption(Nation *nation, int consumedType, int consumingType, int cost);

/*Calculates the desired consumption levels for the turn*/
void calculate_resource_consumption(Nation *nation) {
    int remainingGdp = nation->gdp;
    /*Reset consumption*/
    for (int i = 0; i < nation->stateCount; i++) {
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            nation->states[i].resources[j].consumption = 0;
        }
    }

    /*Calculate consumption by industries*/
    calculate_industry_consumptions(nation);
    /*Subtract total from gdp to get population spending*/
    /*Calculate consumption from population spending*/
    for (int i = 0; i < nation->stateCount; i++) {
    }
}

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

void calculate_economic_growth(Nation *nation) {
    int growthFactor = nation->stability * ECONOMIC_GROWTH_FROM_STABILITY + nation->hdi * ECONOMIC_GROWTH_FROM_HDI;
    nation->economicGrowth = growthFactor * ECONOMIC_GROWTH_MAX;
}

/*Not complete*/
void calculate_production_change(Nation *nation) {
    int gdpGrowth = (int)(nation->gdp * nation->economicGrowth);
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        int productionSurplus = nation->exports[i].surplus - nation->exports[i].exported;
        if (productionSurplus > 0) {}

        for (int j = 0; j < nation->stateCount; j++) {
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

void calculate_gdp(Nation *nation) {
    int gdp = 0;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        gdp += nation->resources[i].production;
    }

    nation->gdp = gdp;
    nation->gdpPerCapita = gdp / nation->population;
}

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

void calculate_industry_consumptions(Nation *nation) {
    calculate_industry_consumption(nation, CEREALS, MEAT, CEREALS_MEAT);
    calculate_industry_consumption(nation, CEREALS, DAIRY, CEREALS_DAIRY);
    calculate_industry_consumption(nation, MACHINERY, FOSSIL_FUELS, MACHINERY_FOSSIL_FUELS);
    calculate_industry_consumption(nation, MACHINERY, WOOD_AND_PAPER, MACHINERY_WOOD_AND_PAPER);
    calculate_industry_consumption(nation, ELECTRICITY, WOOD_AND_PAPER, ELECTRICITY_WOOD_AND_PAPER);
    calculate_industry_consumption(nation, MACHINERY, MINERALS, MACHINERY_MINERALS);
    calculate_industry_consumption(nation, ELECTRICITY, IRON_AND_STEEL, ELECTRICITY_IRON_AND_STEEL);
    calculate_industry_consumption(nation, MACHINERY, PRECIOUS_STONES, MACHINERY_PRECIOUS_STONES);
    calculate_industry_consumption(nation, ELECTRICITY, FABRICS, ELECTRICITY_FABRICS);
    calculate_industry_consumption(nation, CHEMICALS, PLASTICS, CHEMICALS_PLASTICS);
    calculate_industry_consumption(nation, ELECTRICITY, PLASTICS, ELECTRICITY_PLASTICS);
    calculate_industry_consumption(nation, ELECTRICITY, CHEMICALS, ELECTRICITY_CHEMICALS);

    calculate_industry_consumption(nation, CHEMICALS, PHARMACEUTICALS, CHEMICALS_PHARMACEUTICALS);
    calculate_industry_consumption(nation, ELECTRICITY, PHARMACEUTICALS, ELECTRICITY_PHARMACEUTICALS);

    calculate_industry_consumption(nation, IRON_AND_STEEL, APPLIANCES, IRON_AND_STEEL_APPLIANCES);
    calculate_industry_consumption(nation, ELECTRICITY, APPLIANCES, ELECTRICITY_APPLIANCES);

    calculate_industry_consumption(nation, IRON_AND_STEEL, VEHICLES, IRON_AND_STEEL_VEHICLES);
    calculate_industry_consumption(nation, PLASTICS, VEHICLES, PLASTICS_VEHICLES);
    calculate_industry_consumption(nation, ELECTRICITY, VEHICLES, ELECTRICITY_VEHICLES);

    calculate_industry_consumption(nation, IRON_AND_STEEL, MACHINERY, IRON_AND_STEEL_MACHINERY);
    calculate_industry_consumption(nation, ELECTRICITY, MACHINERY, ELECTRICITY_MACHINERY);

    calculate_industry_consumption(nation, PLASTICS, COMMODITIES, PLASTICS_COMMODITIES);
    calculate_industry_consumption(nation, CHEMICALS, COMMODITIES, CHEMICALS_COMMODITIES);
    calculate_industry_consumption(nation, FABRICS, COMMODITIES, FABRICS_COMMODITIES);
    calculate_industry_consumption(nation, ELECTRICITY, COMMODITIES, ELECTRICITY_COMMODITIES);

    calculate_industry_consumption(nation, IRON_AND_STEEL, CONSTRUCTION, IRON_AND_STEEL_CONSTRUCTION);
    calculate_industry_consumption(nation, WOOD_AND_PAPER, CONSTRUCTION, WOOD_AND_PAPER_CONSTRUCTION);
    calculate_industry_consumption(nation, MACHINERY, CONSTRUCTION, MACHINERY_CONSTRUCTION);

    calculate_industry_consumption(nation, PHARMACEUTICALS, HEALTHCARE, PHARMACEUTICALS_HEALTHCARE);
    calculate_industry_consumption(nation, MARKETING, RETAIL, MARKETING_RETAIL);
}

void calculate_industry_consumption(Nation *nation, int consumedType, int consumingType, int cost) {
    float costRatio = cost / UNITS_OUTPUT_PER_INPUT;
    for (int i = 0; i < nation->stateCount; i++) {
        nation->resources[consumedType].consumption += (int)(nation->resources[consumingType].production * costRatio);
    }
}