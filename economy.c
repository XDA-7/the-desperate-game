#include "stdlib.h"
#include "constants.h"
#include "data_structures.h"
#include "economy.h"
#include "queries.h"

float produce_resource(Nation *nation, State *state, int type);
/*All consumption calculations are applied on the states*/
float calculate_industry_consumptions(Nation *nation);
float calculate_industry_consumption(Nation *nation, int consumedType, int consumingType, int cost);
void calculate_population_consumption(Nation *nation, float spending);
void calculate_pop_class_consumption(Nation *nation, float spending, ClassSpendingProfile spendingProfile);
void calculate_pop_class_category_consumption(Nation *nation, float spending, int categoryCount, const int *categories);
void calculate_production_priorities(Nation *nation, float *resourcePriorities);
int nation_can_produce_resource(Nation *nation, int resourceType);
int get_resource_producing_states(Nation *nation, int *boolArray, int resourceType);

/*Calculates the desired consumption levels for the turn*/
void calculate_resource_consumption(Nation *nation) {
    float remainingGdp = nation->gdp;
    /*Reset consumption*/
    for (int i = 0; i < nation->stateCount; i++) {
        for (int j = 0; j < RESOURCE_COUNT; j++) {
            nation->states[i].resources[j].consumption = 0;
        }
    }

    /* 
     * Calculate consumption by industries
     * Subtract total from gdp to get population spending
     */
    remainingGdp -= calculate_industry_consumptions(nation);
    // printf("Remaining GDP: %f\n", remainingGdp);
    // for (int i = 0; i < RESOURCE_COUNT; i++) {
    //     printf("Consumption %d: %f\n", i, nation->states[0].resources[i].consumption);
    // }

    /*Calculate consumption from population spending*/
    calculate_population_consumption(nation, remainingGdp);
    // for (int i = 0; i < RESOURCE_COUNT; i++) {
    //     printf("Consumption %d: %f\n", i, nation->states[0].resources[i].consumption);
    // }
    // printf("\n");
    /*Calculate national consumption from states*/
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        nation->resources[i].consumption = 0;
        for (int j = 0; j < nation->stateCount; j++) {
            nation->resources[i].consumption +=
                nation->states[j].resources[i].consumption;
        }
    }
}

void run_national_production(Nation *nation) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        float resourceBalance = 0;
        float production = 0;
        float consumption = 0;
        for (int j = 0; j < nation->stateCount; j++) {
            float stateProduction = produce_resource(nation, &nation->states[j], i);
            float stateConsumption = nation->states[j].resources[i].consumption;
            production += stateProduction;
            // printf("Base state production: %f\n", nation->states[j].resources[i].production);
            // printf("State production: %f\n", stateProduction);
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
        float exports = 0;
        float imports = 0;
        for (int j = 0; j < rMarket.exportCount; j++) {
            exports += rMarket.exports[j]->surplus - rMarket.exports[j]->exported;
        }

        for (int j = 0; j < rMarket.importCount; j++) {
            imports += rMarket.imports[j]->deficit - rMarket.imports[j]->imported;
        }

        float importSatisfaction = exports / imports;
        /*Imports are filled, exports are rationed*/
        if (importSatisfaction >= 1.0f) {
            for (int j = 0; j < rMarket.importCount; j++) {
                rMarket.imports[j]->imported = rMarket.imports[j]->deficit;
            }

            float exportSatisfaction = imports / exports;
            for (int j = 0; j < rMarket.exportCount; j++) {
                float surplus = rMarket.exports[j]->surplus - rMarket.exports[j]->exported;
                rMarket.exports[j]->exported += (surplus * exportSatisfaction);
            }
        }
        /*Exports are filled, imports are rationed*/
        else {
            for (int j = 0; j < rMarket.exportCount; j++) {
                rMarket.exports[j]->exported = rMarket.exports[j]->surplus;
            }

            for (int j = 0; j < rMarket.importCount; j++) {
                float deficit = rMarket.imports[j]->deficit - rMarket.imports[j]->imported;
                rMarket.imports[j]->imported += (deficit * importSatisfaction);
            }
        }
    }
}

void calculate_economic_growth(Nation *nation) {
    float growthFactor = nation->stability * ECONOMIC_GROWTH_FROM_STABILITY + nation->hdi * ECONOMIC_GROWTH_FROM_HDI;
    nation->economicGrowth = growthFactor * ECONOMIC_GROWTH_MAX;
}

void calculate_resource_satisfaction(Nation *nation) {
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        float resourceSupply = nation->resources[i].production + nation->imports[i].imported;
        float resourceDemand = nation->resources[i].consumption;
        // printf("Resource %d:\nSupply: %f\nDemand: %f\n", i, resourceSupply, resourceDemand);
        if (resourceSupply < resourceDemand) {
            nation->resourceSatisfaction[i] = resourceSupply / resourceDemand;
            nation->resourceShortage[i] = resourceDemand - resourceSupply;
        }
        else {
            nation->resourceSatisfaction[i] = 1.0f;
            nation->resourceShortage[i] = 0.0f;
        }
    }
}

/*
 * Growth is divided according to deficits in each sector for the nation as a whole.
 * The growth is then divided among all states that can produce the resource.
 */
void calculate_production_change(Nation *nation) {
    float gdpIncrease = nation->economicGrowth * nation->gdp;
    float resourcePriorities[RESOURCE_COUNT];
    calculate_production_priorities(nation, resourcePriorities);
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (resourcePriorities[i] == 0.0f) {
            continue;
        }

        int *isProducingState = malloc(nation->stateCount * sizeof(int));
        int producingStateCount = get_resource_producing_states(nation, isProducingState, i);
        float productionIncreasePerState = (gdpIncrease * resourcePriorities[i]) / producingStateCount;
        // printf("Production increase per state for %d: %f\n", i, productionIncreasePerState);
        // printf("Producing state count: %d\n", producingStateCount);
        // printf("Resource priority: %f\n", resourcePriorities[i]);
        for (int j = 0; j < nation->stateCount; j++) {
            if (isProducingState[j]) {
                //printf("Production before: %f\n", nation->states[j].resources[i].production);
                nation->states[j].resources[i].production += productionIncreasePerState;
                // printf("Production after: %f\n", nation->states[j].resources[i].production);
            }
        }

        // printf("End resource increase %d\n\n", i);
        free(isProducingState);
    }
}

void calculate_gdp(Nation *nation) {
    float gdp = 0;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        gdp += nation->resources[i].production;
        // printf("%f\n", nation->resources[i].production);
    }
    // printf("\n");

    nation->gdp = gdp;
    nation->gdpPerCapita = gdp / nation->population;
}

/*Production of a resource is proportional to the average resource satisfactions of any dependent resources*/
float produce_resource(Nation *nation, State *state, int type) {
    float production = state->resources[type].production;
    switch (type)
    {
        case MEAT:
            return production * nation->resourceSatisfaction[CEREALS];
        case DAIRY:
            return production * nation->resourceSatisfaction[CEREALS];
        case FOSSIL_FUELS:
            return production * nation->resourceSatisfaction[MACHINERY];
        case WOOD_AND_PAPER:
            return production * ((
                nation->resourceSatisfaction[MACHINERY] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2);
        case MINERALS:
            return production * nation->resourceSatisfaction[MACHINERY];
        case IRON_AND_STEEL:
            return production * nation->resourceSatisfaction[ELECTRICITY];
        case PRECIOUS_STONES:
            return production * nation->resourceSatisfaction[MACHINERY];
        case FABRICS:
            return production * nation->resourceSatisfaction[ELECTRICITY];
        case PLASTICS:
            return production * ((
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2);
        case CHEMICALS:
            return production * nation->resourceSatisfaction[ELECTRICITY];
        case PHARMACEUTICALS:
            return production * ((
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2);
        case APPLIANCES:
            return production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2);
        case VEHICLES:
            return production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[PLASTICS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 3);
        case MACHINERY:
            return production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 2);
        case COMMODITIES:
            return production * ((
                nation->resourceSatisfaction[PLASTICS] +
                nation->resourceSatisfaction[CHEMICALS] +
                nation->resourceSatisfaction[FABRICS] +
                nation->resourceSatisfaction[ELECTRICITY]
            ) / 4);
        case CONSTRUCTION:
            return production * ((
                nation->resourceSatisfaction[IRON_AND_STEEL] +
                nation->resourceSatisfaction[WOOD_AND_PAPER] +
                nation->resourceSatisfaction[MACHINERY]
            ) / 3);
        case HEALTHCARE:
            return production * nation->resourceSatisfaction[PHARMACEUTICALS];
        case RETAIL:
            return production * nation->resourceSatisfaction[MARKETING];
        default:
            return production;
    }
}

float calculate_industry_consumptions(Nation *nation) {
    float totalConsumption = 0;

    totalConsumption += calculate_industry_consumption(nation, CEREALS, MEAT, CEREALS_MEAT);
    totalConsumption += calculate_industry_consumption(nation, CEREALS, DAIRY, CEREALS_DAIRY);
    totalConsumption += calculate_industry_consumption(nation, MACHINERY, FOSSIL_FUELS, MACHINERY_FOSSIL_FUELS);
    totalConsumption += calculate_industry_consumption(nation, MACHINERY, WOOD_AND_PAPER, MACHINERY_WOOD_AND_PAPER);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, WOOD_AND_PAPER, ELECTRICITY_WOOD_AND_PAPER);
    totalConsumption += calculate_industry_consumption(nation, MACHINERY, MINERALS, MACHINERY_MINERALS);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, IRON_AND_STEEL, ELECTRICITY_IRON_AND_STEEL);
    totalConsumption += calculate_industry_consumption(nation, MACHINERY, PRECIOUS_STONES, MACHINERY_PRECIOUS_STONES);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, FABRICS, ELECTRICITY_FABRICS);
    totalConsumption += calculate_industry_consumption(nation, CHEMICALS, PLASTICS, CHEMICALS_PLASTICS);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, PLASTICS, ELECTRICITY_PLASTICS);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, CHEMICALS, ELECTRICITY_CHEMICALS);

    totalConsumption += calculate_industry_consumption(nation, CHEMICALS, PHARMACEUTICALS, CHEMICALS_PHARMACEUTICALS);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, PHARMACEUTICALS, ELECTRICITY_PHARMACEUTICALS);

    totalConsumption += calculate_industry_consumption(nation, IRON_AND_STEEL, APPLIANCES, IRON_AND_STEEL_APPLIANCES);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, APPLIANCES, ELECTRICITY_APPLIANCES);

    totalConsumption += calculate_industry_consumption(nation, IRON_AND_STEEL, VEHICLES, IRON_AND_STEEL_VEHICLES);
    totalConsumption += calculate_industry_consumption(nation, PLASTICS, VEHICLES, PLASTICS_VEHICLES);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, VEHICLES, ELECTRICITY_VEHICLES);

    totalConsumption += calculate_industry_consumption(nation, IRON_AND_STEEL, MACHINERY, IRON_AND_STEEL_MACHINERY);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, MACHINERY, ELECTRICITY_MACHINERY);

    totalConsumption += calculate_industry_consumption(nation, PLASTICS, COMMODITIES, PLASTICS_COMMODITIES);
    totalConsumption += calculate_industry_consumption(nation, CHEMICALS, COMMODITIES, CHEMICALS_COMMODITIES);
    totalConsumption += calculate_industry_consumption(nation, FABRICS, COMMODITIES, FABRICS_COMMODITIES);
    totalConsumption += calculate_industry_consumption(nation, ELECTRICITY, COMMODITIES, ELECTRICITY_COMMODITIES);

    totalConsumption += calculate_industry_consumption(nation, IRON_AND_STEEL, CONSTRUCTION, IRON_AND_STEEL_CONSTRUCTION);
    totalConsumption += calculate_industry_consumption(nation, WOOD_AND_PAPER, CONSTRUCTION, WOOD_AND_PAPER_CONSTRUCTION);
    totalConsumption += calculate_industry_consumption(nation, MACHINERY, CONSTRUCTION, MACHINERY_CONSTRUCTION);

    totalConsumption += calculate_industry_consumption(nation, PHARMACEUTICALS, HEALTHCARE, PHARMACEUTICALS_HEALTHCARE);
    totalConsumption += calculate_industry_consumption(nation, MARKETING, RETAIL, MARKETING_RETAIL);

    return totalConsumption;
}

float calculate_industry_consumption(Nation *nation, int consumedType, int consumingType, int cost) {
    float costRatio = cost / (float)UNITS_OUTPUT_PER_INPUT;
    // printf("Cost ratio: %f\n", costRatio);
    float consumption = 0;
    for (int i = 0; i < nation->stateCount; i++) {
        float stateConsumption = nation->states[i].resources[consumingType].production * costRatio;
        // printf("Production of consuming type: %f\n", nation->states[i].resources[consumingType].production);
        // printf("State consumption: %f\n", stateConsumption);
        nation->states[i].resources[consumedType].consumption += stateConsumption;
        consumption += stateConsumption;
    }

    return consumption;
}

void calculate_population_consumption(Nation *nation, float spending) {
    float maxLowIncomeSpending = nation->population * LOW_INCOME_CAP;
    if (spending <= maxLowIncomeSpending) {
        calculate_pop_class_consumption(nation, spending, LOW_INCOME_SPENDING);
        return;
    }

    calculate_pop_class_consumption(nation, maxLowIncomeSpending, LOW_INCOME_SPENDING);
    spending -= maxLowIncomeSpending;
    float maxMiddleIncomeSpending = nation->population * (MIDDLE_INCOME_CAP - LOW_INCOME_CAP);
    if (spending <= maxMiddleIncomeSpending) {
        calculate_pop_class_consumption(nation, spending, MIDDLE_INCOME_SPENDING);
        return;
    }

    calculate_pop_class_consumption(nation, maxMiddleIncomeSpending, MIDDLE_INCOME_SPENDING);
    spending -= maxMiddleIncomeSpending;
    calculate_pop_class_consumption(nation, spending, HIGH_INCOME_SPENDING);
}

void calculate_pop_class_consumption(Nation *nation, float spending, ClassSpendingProfile spendingProfile) {
    float foodSpending = spending * spendingProfile.food;
    float basicSpending = spending * spendingProfile.basicGoods;
    float luxurySpending = spending * spendingProfile.luxuryGoods;

    calculate_pop_class_category_consumption(nation, foodSpending, FOOD_COUNT, FOOD_TYPES);
    calculate_pop_class_category_consumption(nation, basicSpending, BASIC_GOODS_COUNT, BASIC_GOOD_TYPES);
    calculate_pop_class_category_consumption(nation, luxurySpending, LUXURY_GOODS_COUNT, LUXURY_GOOD_TYPES);
}

void calculate_pop_class_category_consumption(Nation *nation, float spending, int categoryCount, const int *categories) {
    // printf("Population: %f\nSpending: %f\nCategory count: %d\n", nation->population, spending, categoryCount);
    float amountPerPersonPerCategory = (spending / categoryCount) / nation->population;
    // printf("AmountPerPersonPerCategory: %f\n", amountPerPersonPerCategory);
    for (int i = 0; i < nation->stateCount; i++) {
        float stateConsumption = nation->states[i].population * amountPerPersonPerCategory;
        for (int j = 0; j < categoryCount; j++) {
            nation->states[i].resources[categories[j]].consumption += stateConsumption;
        }
    }
}

/*
 * Priorities are based on the gross good shortage rather than the ratio of supply to demand
 * This is to prevent large gdp growth from oversupplying smaller resource sectors
 */
void calculate_production_priorities(Nation *nation, float *resourcePriorities) {
    float totalShortages = 0.0f;
    int resourceTypesProduced = 0;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        resourcePriorities[i] = 0.0f;
        if (nation_can_produce_resource(nation, i)) {
            resourceTypesProduced++;
            // printf("Nation can produce resource %d\n", i);
            resourcePriorities[i] = nation->resourceShortage[i];
            totalShortages += nation->resourceShortage[i];
        }
    }

    // printf("TotalShortages: %f\n", totalShortages);
    /*TODO: Production should be increased for resources where there is export demand first*/
    if (totalShortages == 0) {
        float evenPriorities = 1.0 / resourceTypesProduced;
        // printf("Even priority: %f\n", evenPriorities);
        for (int i = 0; i < RESOURCE_COUNT; i++) {
            if (nation_can_produce_resource(nation, i)) {
                resourcePriorities[i] = evenPriorities;
            }
        }
    }
    else {
        /*Normalisation*/
        for (int i = 0; i < RESOURCE_COUNT; i++) {
            // printf("Resource: %d\n", i);
            // printf("Resource priority: %f\n", resourcePriorities[i]);
            resourcePriorities[i] /= totalShortages;
            // printf("Normalised resource priority: %f\n", resourcePriorities[i]);
            // printf("\n");
        }
    }
}

int nation_can_produce_resource(Nation *nation, int resourceType) {
    for (int i = 0; i < nation->stateCount; i++) {
        if (is_resource_produced(&nation->states[i], resourceType)) {
            return 1;
        }
    }

    return 0;
}

/*Returns the number of states producing the resource and modifies the array to identify which states these are*/
int get_resource_producing_states(Nation *nation, int *boolArray, int resourceType) {
    int count = 0;
    for (int i = 0; i < nation->stateCount; i++) {
        if (is_resource_produced(&nation->states[i], resourceType)) {
            count++;
            boolArray[i] = 1;
        }
        else {
            boolArray[i] = 0;
        }
    }

    return count;
}