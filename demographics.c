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
void calculate_happiness(Nation *nation) {}

/*Corruption is a product of government spending and government type*/
void calculate_corruption(Nation *nation) {}

/*Stability is a product of
 *base stability
 *HDI
 *happiness
 *corruption
 *military
 *cultural diversity
 *propaganda*/
void calculate_stability(Nation *nation) {}