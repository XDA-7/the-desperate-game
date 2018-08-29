#include "stdio.h"
#include "stdlib.h"
#include "constants.h"
#include "data_structures.h"
#include "economy.h"
#include "government.h"
#include "utils.h"
#include "world.h"

/*process execution order:
once:
world

loop
economics
government
*/

int main() {
    init_nations();

    return 0;
}