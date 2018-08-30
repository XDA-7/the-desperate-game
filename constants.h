
#ifndef CONSTANTS
#define CONSTANTS
#include "utils.h"

/*Resource types*/
#define RESOURCE_COUNT 26

/*Agriculture*/
#define CEREALS 0
#define VEGETABLES_AND_FRUIT 1
#define MEAT 2
#define DAIRY 3
#define TOBACCO 4
#define DRUGS 5

/*Energy*/
#define ELECTRICITY 6
#define FOSSIL_FUELS 7

/*Raw materials*/
#define WOOD_AND_PAPER 8
#define MINERALS 9
#define IRON_AND_STEEL 10
#define PRECIOUS_STONES 11

/*Industrial goods*/
#define FABRICS 12
#define PLASTICS 13
#define CHEMICALS 14
#define PHARMACEUTICALS 15

/*Finished goods*/
#define APPLIANCES 16
#define VEHICLES 17
#define MACHINERY 18
#define COMMODITIES 19

/*Services*/
#define CONSTRUCTION 20
#define ENGINEERING 21
#define HEALTHCARE 22
#define RETAIL 23
#define LEGAL_SERVICES 24
#define MARKETING 25

/*
 * Resource dependencies
 * These values are used to calculate resource consumption generated by industry
 */
#define UNITS_OUTPUT_PER_INPUT 60

#define CEREALS_MEAT 5
#define CEREALS_DAIRY 5

#define MACHINERY_FOSSIL_FUELS 10

#define MACHINERY_WOOD_AND_PAPER 10
#define ELECTRICITY_WOOD_AND_PAPER 5
#define MACHINERY_MINERALS 10
#define ELECTRICITY_IRON_AND_STEEL 10
#define MACHINERY_PRECIOUS_STONES 5

#define ELECTRICITY_FABRICS 5
#define CHEMICALS_PLASTICS 10
#define ELECTRICITY_PLASTICS 5
#define ELECTRICITY_CHEMICALS 5
#define CHEMICALS_PHARMACEUTICALS 10
#define ELECTRICITY_PHARMACEUTICALS 5

#define IRON_AND_STEEL_APPLIANCES 10
#define ELECTRICITY_APPLIANCES 5
#define IRON_AND_STEEL_VEHICLES 15
#define PLASTICS_VEHICLES 10
#define ELECTRICITY_VEHICLES 5
#define IRON_AND_STEEL_MACHINERY 15
#define ELECTRICITY_MACHINERY 5
#define PLASTICS_COMMODITIES 10
#define CHEMICALS_COMMODITIES 10
#define FABRICS_COMMODITIES 5
#define ELECTRICITY_COMMODITIES 5

#define IRON_AND_STEEL_CONSTRUCTION 15
#define WOOD_AND_PAPER_CONSTRUCTION 10
#define MACHINERY_CONSTRUCTION 5
#define PHARMACEUTICALS_HEALTHCARE 10
#define MARKETING_RETAIL 5

/*World generation*/
#define LARGE_NATION_SIZE 5
#define LARGE_NATION_COUNT 5

#define MEDIUM_NATION_SIZE 3
#define MEDIUM_NATION_COUNT 60

#define SMALL_NATION_SIZE 1
#define SMALL_NATION_COUNT 120

#define NATION_COUNT LARGE_NATION_COUNT + MEDIUM_NATION_COUNT + SMALL_NATION_COUNT
#define STATE_COUNT LARGE_NATION_COUNT * LARGE_NATION_SIZE + MEDIUM_NATION_COUNT * MEDIUM_NATION_SIZE + SMALL_NATION_COUNT * SMALL_NATION_SIZE

#define AGRICULTURE_PER_STATE 2
#define CEREAL_START_MIN 50
#define CEREAL_START_MAX 75

#define FOSSIL_FUEL_CHECK (roll_check(19, 20))

#define RAW_MATERIALS_PER_STATE 1
#define PRECIOUS_STONES_CHECK (roll_check(99, 100))

#define INDUSTRY_PER_STATE 2
#define FINISHED_GOODS_PER_STATE 2

#define STATE_POP_MIN 1
#define STATE_POP_MAX 10

/*Scaling values*/
#define POPULATION_SCALING 1000
#define GDP_SCALING 1000000
#define GDP_POPULATION_RATIO GDP_SCALING / POPULATION_SCALING

#define MIN_CEREAL_CONSUMPTION 3
/*
 * The ratio of spending on each class of resource will vary based on the income per capita
 * Low incomes will have a higher portion of food spending, higher levels of spending will have increasing portions of finished goods followed by services
 */
#define LOW_INCOME_CAP 12
#define MIDDLE_INCOME_CAP 30

#define FOOD_COUNT 4
#define BASIC_GOODS_COUNT 8
#define LUXURY_GOODS_COUNT 6
const int FOOD_TYPES[FOOD_COUNT];
const int BASIC_GOOD_TYPES[BASIC_GOODS_COUNT];
const int LUXURY_GOOD_TYPES[LUXURY_GOODS_COUNT];

#define LOW_INCOME_FOOD 0.95
#define LOW_INCOME_BASIC 0.05
#define LOW_INCOME_LUXURY 0.0
#define MIDDLE_INCOME_FOOD 0.4
#define MIDDLE_INCOME_BASIC 0.4
#define MIDDLE_INCOME_LUXURY 0.2
#define HIGH_INCOME_FOOD 0.1
#define HIGH_INCOME_BASIC 0.3
#define HIGH_INCOME_LUXURY 0.6
typedef struct ClassSpendingProfile
{
    float food;
    float basicGoods;
    float luxuryGoods;
} ClassSpendingProfile;
const ClassSpendingProfile LOW_INCOME_SPENDING;
const ClassSpendingProfile MIDDLE_INCOME_SPENDING;
const ClassSpendingProfile HIGH_INCOME_SPENDING;

/*Nation performance*/
#define ECONOMIC_GROWTH_MAX 0.04
#define ECONOMIC_GROWTH_FROM_STABILITY 1.0
#define ECONOMIC_GROWTH_FROM_HDI 0.0
#define ECONOMIC_GROWTH_FROM_INFRASTRUCTURE 0.0
#define ECONOMIC_GROWTH_FROM_NO_CORRUPTION 0.0

/*Nation stat modifiers*/
/*HDI*/
#define MAX_HDI_FROM_HEALTHCARE 0.5
#define MAX_HDI_FROM_EDUCATION 0.5
#define MAX_EFFECTIVE_HEALTHCARE_PER_CAPITA 12000 / GDP_POPULATION_RATIO
#define MAX_EFFECTIVE_EDUCATION_PER_CAPITA 12000 / GDP_POPULATION_RATIO

/*Happiness*/
#define MAX_HAPPINESS_FROM_HDI 0.3
#define MAX_HAPPINESS_FROM_RESOURCES 0.3
#define MIN_HAPPINESS_FROM_RESOURCES -0.2
#define HAPPINESS_LOSS_PER_MISSING_RESOURCE 0.1
#define MAX_HAPPINESS_FROM_GOVERNMENT_TYPE 0.3
#define MAX_HAPPINESS_FROM_INCOME 0.3
#define MAX_HAPPINESS_INCOME_PER_CAPITA 90000 / GDP_POPULATION_RATIO
#define MAX_UNHAPPINESS_FROM_WAR 0.5

/*Corruption*/
#define MAX_CORRUPTION_LOSS_FROM_SPENDING 0.5
#define MAX_EFFECTIVE_GOVERNMENT_PER_CAPITA 5000 / GDP_POPULATION_RATIO
#define MAX_CORRUPTION_FROM_GOVERNMENT 1.0

/*Stability*/
#define MAX_STABILITY_FROM_GOVERNMENT 0.5
#define MAX_STABILITY_FROM_HDI 0.2
#define MAX_STABLILITY_FROM_PROPAGANDA 0.3
#define MAX_INSTABILITY_FROM_UNHAPPINESS 0.25
#define MAX_INSTABILITY_FROM_CORRUPTION 0.4
#define MAX_INSTABILITY_FROM_MILITARY 0.4
#define MAX_INSTABILITY_MILITARY 30000 / GDP_POPULATION_RATIO
#define MAX_INSTABILITY_FROM_DIVERSITY 0.4

/*Regime change*/
#define COUP_STABILITY_THRESHOLD 0.2
#define MAX_COUP_PROBABILITY 0.25
#define REVOLUTION_HAPPINESS_THRESHOLD 0.2
#define MAX_REVOLUTION_PROBABLILITY 0.2

/*Government types*/
#define GOVERNMENT_TYPES 5 /*Anarchy will be treated specially*/
#define DEMOCRACY 0
#define COMMUNISM 1
#define FASCISM 2
#define MONARCHY 3
#define THEOCRACY 4
#define ANARCHY 5
typedef struct GovernmentStats {
    float corruptionModifier;
    float economicGrowthModifier;
    float relationsModifier;
    float happinessModifier;
    float instabilityFromUnhappiness;
    float instabilityFromMilitary;
    float unhappinessFromWar;
    /*Coups always lead to fascism*/
    float coupRisk;
    /*Revolution can lead to any of the types*/
    float revolutionRisk;
    float baseStabilityIncrease;
    float baseStability;
    float instabilityFromDifferentCultures;
    float instabilityFromAssimilation;
    float instabilityFromExtermination;
} GovernmentStats;
const GovernmentStats GOVERNMENT_STATS[GOVERNMENT_TYPES];

typedef struct GovernmentTraits {
    int hasElections:1;
    int canAnnexWithoutClaim:1;
    int canPuppet:1;
    int canInstallType:1;
    int canExterminateCulture:1;
    int hasImprovedRelationsWithSameGovernmentType:1;
    int canChangeGovernmentType:1;
    int coupDoesntChangeGovernmentType:1;
} GovernmentTraits;
const GovernmentTraits GOVERNMENT_TRAITS[GOVERNMENT_TYPES];

/*Government spending*/
#define GOVERNMENT_SPENDING_AREAS 7
#define INFRASTRUCTURE 0
#define MILITARY 1
#define INTELLIGENCE 2
#define ADMINISTRATION 3
#define PROPAGANDA 4
#define HEALTHCARE_SPENDING 5 /*Disambiguation with healthcare resource*/
#define EDUCATION 6

#define MAX_OVERSPENDING 1.5
#define MIN_UNDERSPENDING 0.5
/*Increase in base spending caused by 100% overspending*/
#define SPENDING_STIMULATION 0.3
#define DEBT_INTEREST 0.05
#define MAX_CORRUPTION_REVENUE_LOSS 0.4

#endif