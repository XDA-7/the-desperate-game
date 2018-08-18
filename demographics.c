#include "data_structures.h"

void calculate_population(Nation *nation) {
    float population = 0;
    for (int i = 0; i < nation->stateCount; i++) {
        population += nation->states[i].population;
    }

    nation->population = population;
}