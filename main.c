#include "stdio.h"
#include "stdlib.h"
#include "constants.h"
#include "data_structures.h"
#include "economy.h"
#include "utils.h"
#include "world.h"

/*process execution order:
once:
world
calculate_population
calculate_gdp

loop
economics
*/

int main() {
    init_nations();
    // printf("%d\n");

    return 0;
}