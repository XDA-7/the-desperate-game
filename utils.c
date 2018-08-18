#include "stdlib.h"
#include "utils.h"

int roll_check(int requiredValue, int maxValue) {
    return (rand() % maxValue) >= requiredValue;
}

int roll(int values) {
    return rand() % values;
}

int select_value(int count, int* values) {
    return values[rand() % count];
}

int select_range(int min, int max) {
    int variation = rand() % (max - min);
    return min + variation;
}

int is_resource_produced(State *state, int type) {
    int typeBit = 1 << type;
    return state->producedResources & typeBit;
}

void enable_resource_production(State *state, int type) {
    int typeBit = 1 << type;
    state->producedResources = state->producedResources | typeBit;
}