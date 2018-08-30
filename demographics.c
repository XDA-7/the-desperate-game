#include "data_structures.h"

void calculate_population(Nation *nation) {
    float population = 0;
    for (int i = 0; i < nation->stateCount; i++) {
        population += nation->states[i].population;
    }

    nation->population = population;
}

/*HDI is a product of education and health spending per capita*/
void calculate_hdi(Nation *nation) {
    float healthPerCapita =
        nation->government.spending[HEALTHCARE_SPENDING].current / nation->population;
    float healthModifier = healthPerCapita > MAX_EFFECTIVE_HEALTHCARE_PER_CAPITA ?
        1.0f :
        healthPerCapita / (float)MAX_EFFECTIVE_HEALTHCARE_PER_CAPITA;
    float educationPerCapita =
        nation->government.spending[EDUCATION].current / nation->population;
    float educationModifier = educationPerCapita > MAX_EFFECTIVE_EDUCATION_PER_CAPITA ?
        1.0f :
        educationPerCapita / (float)MAX_EFFECTIVE_EDUCATION_PER_CAPITA;
    nation->hdi =
        healthModifier * MAX_HDI_FROM_HEALTHCARE +
        educationModifier * MAX_HDI_FROM_EDUCATION;
}

/*Happiness is a product of
 *HDI
 *resource satisfaction
 *government type
 *GDP per capita
 *war*/
void calculate_happiness(Nation *nation) {
    float gainFromHdi = nation->hdi * MAX_HAPPINESS_FROM_HDI;
    float gainFromResources = MAX_HAPPINESS_FROM_RESOURCES;
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        float resourceLoss =
            (1.0f - nation->resourceSatisfaction[i]) *
            HAPPINESS_LOSS_PER_MISSING_RESOURCE;
        gainFromResources -= resourceLoss;
        if (gainFromResources < MIN_HAPPINESS_FROM_RESOURCES) {
            gainFromResources = MIN_HAPPINESS_FROM_RESOURCES;
            break;
        }
    }

    float gainFromGovernment =
        MAX_HAPPINESS_FROM_GOVERNMENT_TYPE *
        GOVERNMENT_STATS[nation->government.governmentType].happinessModifier;
    float gainFromGdp = (nation->gdpPerCapita / MAX_HAPPINESS_INCOME_PER_CAPITA) *
        MAX_HAPPINESS_FROM_INCOME;
    /*TODO: War loss*/
    float totalHappiness = gainFromHdi + gainFromResources + gainFromGovernment + gainFromGdp;
    nation->happiness = totalHappiness > 1.0f ? 1.0f : totalHappiness;
}

/*Corruption is a product of government spending and government type*/
void calculate_corruption(Nation *nation) {
    float corruptionFromGovernment = MAX_CORRUPTION_FROM_GOVERNMENT *
        GOVERNMENT_STATS[nation->government.governmentType].corruptionModifier;
    float governmentPerCapita = nation->government.spending[ADMINISTRATION].current /
        nation->population;
    float lossFromSpending = (governmentPerCapita / MAX_EFFECTIVE_GOVERNMENT_PER_CAPITA) *
        MAX_CORRUPTION_LOSS_FROM_SPENDING;
    float totalCorruption = corruptionFromGovernment - lossFromSpending;
    nation->corruption = totalCorruption < 0.0f ? 0.0f : totalCorruption;
}

/*Stability is a product of
 *base stability
 *HDI
 *happiness
 *corruption
 *military
 *cultural diversity
 *propaganda*/
void calculate_stability(Nation *nation) {}