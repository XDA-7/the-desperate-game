#include "constants.h"

const int FOOD_TYPES[] = { CEREALS, VEGETABLES_AND_FRUIT, DAIRY, MEAT };
const int BASIC_GOOD_TYPES[] = { TOBACCO, DRUGS, ELECTRICITY, FOSSIL_FUELS, MINERALS, WOOD_AND_PAPER, COMMODITIES, RETAIL };
const int LUXURY_GOOD_TYPES[] = { PRECIOUS_STONES, APPLIANCES, VEHICLES, PHARMACEUTICALS, HEALTHCARE, LEGAL_SERVICES };

const ClassSpendingProfile LOW_INCOME_SPENDING = { LOW_INCOME_FOOD, LOW_INCOME_BASIC, LOW_INCOME_LUXURY  };
const ClassSpendingProfile MIDDLE_INCOME_SPENDING = { MIDDLE_INCOME_FOOD, MIDDLE_INCOME_BASIC, MIDDLE_INCOME_LUXURY };
const ClassSpendingProfile HIGH_INCOME_SPENDING = { HIGH_INCOME_FOOD, HIGH_INCOME_BASIC, HIGH_INCOME_LUXURY };

#define HIGH 0.8
#define MEDIUM 0.5
#define LOW 0.2
#define HIGH_BASE_STABILITY 0.5
#define MEDIUM_BASE_STABILITY 0.3
#define LOW_BASE_STABILITY 0.1

/*corruption*/
/*economicGrowth*/
/*relations*/
/*happiness*/
/*instabilityFromUnhappiness*/
/*instabilityFromMilitary*/
/*unhappinessFromWar*/
/*coupRisk*/
/*revolutionRisk*/
/*baseStability*/
/*instabilityFromDifferentCultures*/
/*instabilityFromAssimilation*/
/*instabilityFromExtermination*/
const GovernmentStats GOVERNMENT_STATS[GOVERNMENT_TYPES] = {
    /*Democracy*/
    {
        /*corruption*/
        LOW,
        /*economicGrowth*/
        HIGH,
        /*relations*/
        HIGH, /*Least likely form of government to start wars*/
        /*happiness*/
        HIGH,
        /*instabilityFromUnhappiness*/
        LOW, /*Discontent citizens prefer to protest rather than rebel*/
        /*instabilityFromMilitary*/
        MEDIUM,
        /*unhappinessFromWar*/
        HIGH,
        /*coupRisk*/
        LOW,
        /*revolutionRisk*/
        LOW,
        /*baseStability*/
        LOW_BASE_STABILITY, /*Democracy is inherently unstable*/
        /*instabilityFromDifferentCultures*/
        LOW,
        /*instabilityFromAssimilation*/
        MEDIUM,
        /*instabilityFromExtermination*/
        HIGH
    },
    /*Communism*/
    {
        /*corruption*/
        MEDIUM,
        /*economicGrowth*/
        LOW, /*Hurr durr communism makes people poor*/
        /*relations*/
        LOW, /*Ideological expansionist*/
        /*happiness*/
        MEDIUM,
        /*instabilityFromUnhappiness*/
        LOW, /*Strong controls of the population stifle dissent*/
        /*instabilityFromMilitary*/
        HIGH, /*Communist governments have a lot of internal struggles*/
        /*unhappinessFromWar*/
        LOW,
        /*coupRisk*/
        MEDIUM,
        /*revolutionRisk*/
        LOW, /*Citizen's freedom of assembly and armament is heavily restricted*/
        /*baseStability*/
        MEDIUM,
        /*instabilityFromDifferentCultures*/
        MEDIUM, /*Foreign ideologies are introduced*/
        /*instabilityFromAssimilation*/
        LOW,
        /*instabilityFromExtermination*/
        MEDIUM
    },
    /*Fascism*/
    {
        /*corruption*/
        MEDIUM,
        /*economicGrowth*/
        MEDIUM,
        /*relations*/
        LOW, /*Territorial expansionist*/
        /*happiness*/
        LOW,
        /*instabilityFromUnhappiness*/
        LOW, /*Strong controls of the population stifle dissent*/
        /*instabilityFromMilitary*/
        LOW,
        /*unhappinessFromWar*/
        LOW,
        /*coupRisk*/
        HIGH,
        /*revolutionRisk*/
        LOW, /*Citizen's freedom of assembly and armament is heavily restricted*/
        /*baseStability*/
        MEDIUM,
        /*instabilityFromDifferentCultures*/
        MEDIUM,
        /*instabilityFromAssimilation*/
        LOW,
        /*instabilityFromExtermination*/
        LOW
    },
    /*Monarchy*/
    {
        /*corruption*/
        HIGH, /*Government is absolute and also quite antiquated/inefficient*/
        /*economicGrowth*/
        MEDIUM,
        /*relations*/
        MEDIUM,
        /*happiness*/
        MEDIUM,
        /*instabilityFromUnhappiness*/
        HIGH, /*A monarch makes a good focal point for resentment*/
        /*instabilityFromMilitary*/
        HIGH,
        /*unhappinessFromWar*/
        MEDIUM,
        /*coupRisk*/
        LOW,
        /*revolutionRisk*/
        HIGH,
        /*baseStability*/
        HIGH, /*Tradition is a powerful force in a monarchy*/
        /*instabilityFromDifferentCultures*/
        LOW,
        /*instabilityFromAssimilation*/
        MEDIUM,
        /*instabilityFromExtermination*/
        HIGH
    },
    /*Theocracy*/
    {
        /*corruption*/
        MEDIUM, /*Religious zeal keeps the worst excesses in check*/
        /*economicGrowth*/
        MEDIUM,
        /*relations*/
        LOW, /*Technically in religious war with rest of the world*/
        /*happiness*/
        MEDIUM,
        /*instabilityFromUnhappiness*/
        MEDIUM, /*Divine right can make for a powerful deterrent*/
        /*instabilityFromMilitary*/
        MEDIUM,
        /*unhappinessFromWar*/
        LOW,
        /*coupRisk*/
        LOW,
        /*revolutionRisk*/
        MEDIUM,
        /*baseStability*/
        HIGH,
        /*instabilityFromDifferentCultures*/
        HIGH,
        /*instabilityFromAssimilation*/
        LOW,
        /*instabilityFromExtermination*/
        MEDIUM
    }
};

/*hasElections*/
/*canAnnexWithoutClaim*/
/*canPuppet*/
/*canInstallType*/
/*canExterminateCulture*/
/*hasImprovedRelationsWithSameGovernmentType*/
/*canChangeGovernmentType*/
/*coupDoesntChangeGovernmentType*/
const GovernmentTraits GOVERNMENT_TRAITS[GOVERNMENT_TYPES] = {
    /*Democracy*/
    {
        /*hasElections*/
        1,
        /*canAnnexWithoutClaim*/
        0,
        /*canPuppet*/
        0,
        /*canInstallType*/
        1,
        /*canExterminateCulture*/
        0,
        /*hasImprovedRelationsWithSameGovernmentType*/
        1,
        /*canChangeGovernmentType*/
        0,
        /*coupDoesntChangeGovernmentType*/
        0
    },
    /*Communism*/
    {
        /*hasElections*/
        0,
        /*canAnnexWithoutClaim*/
        0,
        /*canPuppet*/
        1,
        /*canInstallType*/
        1,
        /*canExterminateCulture*/
        1,
        /*hasImprovedRelationsWithSameGovernmentType*/
        1,
        /*canChangeGovernmentType*/
        0,
        /*coupDoesntChangeGovernmentType*/
        1
    },
    /*Fascism*/
    {
        /*hasElections*/
        0,
        /*canAnnexWithoutClaim*/
        1,
        /*canPuppet*/
        0,
        /*canInstallType*/
        0,
        /*canExterminateCulture*/
        1,
        /*hasImprovedRelationsWithSameGovernmentType*/
        1,
        /*canChangeGovernmentType*/
        1,
        /*coupDoesntChangeGovernmentType*/
        1
    },
    /*Monarchy*/
    {
        /*hasElections*/
        0,
        /*canAnnexWithoutClaim*/
        1,
        /*canPuppet*/
        1, /*Vassals*/
        /*canInstallType*/
        1,
        /*canExterminateCulture*/
        1,
        /*hasImprovedRelationsWithSameGovernmentType*/
        1,
        /*canChangeGovernmentType*/
        1,
        /*coupDoesntChangeGovernmentType*/
        0
    },
    /*Theocracy*/
    {
        /*hasElections*/
        0,
        /*canAnnexWithoutClaim*/
        1,
        /*canPuppet*/
        0,
        /*canInstallType*/
        0,
        /*canExterminateCulture*/
        1,
        /*hasImprovedRelationsWithSameGovernmentType*/
        0, /*Other theocracies are heathens*/
        /*canChangeGovernmentType*/
        0,
        /*coupDoesntChangeGovernmentType*/
        0
    }
};