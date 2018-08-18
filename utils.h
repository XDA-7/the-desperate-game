#ifndef UTILS
#define UTILS

#include "data_structures.h"

int roll_check(int requiredValue, int maxValue);
int roll(int values);
int select_value(int count, int* values);
int select_range(int min, int max);

int is_resource_produced(State *state, int type);
void enable_resource_production(State *state, int type);

#endif