#ifndef ECONOMY
#define ECONOMY
#include "data_structures.h"

/*These functions should be called in the order they are presented in here*/
void calculate_resource_consumption(Nation *nation);
void run_national_production(Nation *nation);
void run_market(Market *market);
void calculate_resource_satisfaction(Nation *nation);
void calculate_economic_growth(Nation *nation);
void calculate_gdp(Nation *nation);
void calculate_production(Nation *nation);

#endif