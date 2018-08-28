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
        totalSpending += base * modifier;
        /*Expand/contract base*/
        float spendingChange = (modifier - 1.0f) * SPENDING_STIMULATION;
        nation->government.spending[i].base *= (1.0f + spendingChange); 
    }

    /*Debt interest*/

    /*Treaty payments*/

    nation->government.totalSpending += totalSpending;
}

void modify_treasury(Nation *nation) {}