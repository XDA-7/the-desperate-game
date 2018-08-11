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