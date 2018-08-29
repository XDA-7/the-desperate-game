#include "data_structures.h"

void calculate_revenue(Nation *nation) {
    nation->government.revenue = nation->gdp * nation->government.taxRate;
}

void calculate_spending(Nation *nation) {
    float totalSpending = 0.0f;
    /*Government discretionary spending*/
    for (int i = 0; i < GOVERNMENT_SPENDING_AREAS; i++) {
        float base = nation->government.spending[i].base;
        float modifier = nation->government.spending[i].modifier;
        float current = base * modifier;
        nation->government.spending[i].current = current;
        totalSpending += current;
        /*Expand/contract base*/
        float spendingChange = (modifier - 1.0f) * SPENDING_STIMULATION;
        nation->government.spending[i].base *= (1.0f + spendingChange); 
    }

    /*Debt interest*/
    if (nation->government.treasury < 0) {
        totalSpending -= nation->government.treasury * DEBT_INTEREST;
    }

    /*TODO: Corruption*/

    /*TODO: Treaty payments*/

    nation->government.totalSpending += totalSpending;
}

void modify_treasury(Nation *nation) {
    nation->government.treasury +=
        nation->government.revenue - nation->government.totalSpending;
}