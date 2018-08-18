#include "constants.h"

const int FOOD_TYPES[] = { CEREALS, VEGETABLES_AND_FRUIT, DAIRY, MEAT };
const int BASIC_GOOD_TYPES[] = { TOBACCO, DRUGS, ELECTRICITY, FOSSIL_FUELS, MINERALS, WOOD_AND_PAPER, COMMODITIES, RETAIL };
const int LUXURY_GOOD_TYPES[] = { PRECIOUS_STONES, APPLIANCES, VEHICLES, PHARMACEUTICALS, HEALTHCARE, LEGAL_SERVICES };

const ClassSpendingProfile LOW_INCOME_SPENDING = { LOW_INCOME_FOOD, LOW_INCOME_BASIC, LOW_INCOME_LUXURY  };
const ClassSpendingProfile MIDDLE_INCOME_SPENDING = { MIDDLE_INCOME_FOOD, MIDDLE_INCOME_BASIC, MIDDLE_INCOME_LUXURY };
const ClassSpendingProfile HIGH_INCOME_SPENDING = { HIGH_INCOME_FOOD, HIGH_INCOME_BASIC, HIGH_INCOME_LUXURY };