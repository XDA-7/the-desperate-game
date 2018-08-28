#include "data_structures.h"

int is_resource_produced(State *state, int type) {
    int typeBit = 1 << type;
    return state->producedResources & typeBit;
}

void enable_resource_production(State *state, int type) {
    int typeBit = 1 << type;
    state->producedResources = state->producedResources | typeBit;
}