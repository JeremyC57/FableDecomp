// Behaviour test for 0x006D6FA0 CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount: the row's defines, includes and body verbatim, on
// raw images laid out as the header's asserts and the retail reads give them (spendable pool +0x14, StatExperiencePoints +0x18 and
// TrainableStatLevels +0x24 as STLport vectors, PHeroStatsDef +0x5C; the stats def's StatLevels vector +0xFC of 80-byte CAbilityTrackDef
// {ExperienceStat +0x3C, Abilities +0x40} of 72-byte CAbilityTrackLevelDef {ExperienceCost +0x3C}). Pool + the stat category's experience
// buys successive levels while each cost fits; the remainder is the fraction of the next cost; the maximum level (count - 1) caps the
// answer; an exactly affordable cost is bought.
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CTCHEROEXPERIENCE_CATEGORY_BODY
#define FABLE_RETAIL_CTCHEROEXPERIENCE_GETSTATEXPERIENCE_BODY
#define FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS
#define FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"

long CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat_type, float& remainder)
{
    long general_exp = GetExperiencePointsAvailableToSpend();
    remainder = 0.0f;
    long stat_exp = GetStatExperience(GetCategoryTrainableStatIsIn(stat_type)) + general_exp;
    long LevelUp = 1;
    while (true) {
        if (TrainableStatLevels[stat_type] + LevelUp > GetTrainableStatMaxLevel(stat_type))
            return GetTrainableStatMaxLevel(stat_type) - TrainableStatLevels[stat_type];
        long NextLevelCost = PHeroStatsDef->StatLevels[stat_type].Abilities[TrainableStatLevels[stat_type] + LevelUp].ExperienceCost;
        if (stat_exp < NextLevelCost) {
            remainder = (float)stat_exp / NextLevelCost;
            break;
        } else
            stat_exp -= NextLevelCost;
        LevelUp++;
    }
    LevelUp--;
    return LevelUp;
}

static int g_fail = 0;
static void check(int ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); g_fail = 1; } }
static double g_hero[sizeof(CTCHeroExperience) / 8 + 2];
static double g_def[0x110 / 8];
static double g_tracks[3][80 / 8];
static double g_levels[6][72 / 8];
static long g_statexp[4];
static long g_trainable[3];
static void W32(void* base, int off, long v) { memcpy((char*)base + off, &v, 4); }
static void WP(void* base, int off, void* p) { memcpy((char*)base + off, &p, 4); }
int main()
{
    FABLE_STATIC_ASSERT_AT(1, sizeof(CAbilityTrackLevelDef) == 72);
    memset(g_hero, 0, sizeof(g_hero)); memset(g_def, 0, sizeof(g_def)); memset(g_tracks, 0, sizeof(g_tracks)); memset(g_levels, 0, sizeof(g_levels));
    long costs[6] = { 0, 100, 200, 300, 400, 500 };   // level n costs costs[n]; 6 levels -> max level 5
    for (int i = 0; i < 6; ++i) W32(g_levels[i], 0x3c, costs[i]);
    // stat 2 uses category 3 and the six levels; stat 1 an empty track
    W32(g_tracks[2], 0x3c, 3); WP(g_tracks[2], 0x40, g_levels[0]); WP(g_tracks[2], 0x44, g_levels[6 - 1] + 72 / 8);
    W32(g_tracks[1], 0x3c, 0); WP(g_tracks[1], 0x40, g_levels[0]); WP(g_tracks[1], 0x44, g_levels[0]);
    WP(g_def, 0xfc, g_tracks[0]); WP(g_def, 0x100, g_tracks[3 - 1] + 80 / 8);
    CTCHeroExperience* hero = reinterpret_cast<CTCHeroExperience*>(g_hero);
    WP(g_hero, 0x18, g_statexp); WP(g_hero, 0x1c, g_statexp + 4);
    WP(g_hero, 0x24, g_trainable); WP(g_hero, 0x28, g_trainable + 3);
    WP(g_hero, 0x5c, g_def);
    float rem = -1.0f;
    // level 1, pool 50 + category 3 has 300: next costs 200 (level 2) fits, 300 (level 3) does not: 1 level, 150/300
    g_trainable[2] = 1; W32(g_hero, 0x14, 50); g_statexp[3] = 300; g_statexp[0] = 99999;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 1, "one level");
    check(rem == 0.5f, "remainder towards the next level");
    // exactly affordable: 200 + 300 = 500 buys two levels, remainder 0 of 400
    W32(g_hero, 0x14, 0); g_statexp[3] = 500;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 2 && rem == 0.0f, "exact costs are bought");
    // nothing affordable
    g_statexp[3] = 10;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 0 && rem == 10.0f / 200.0f, "none: fraction of the first cost");
    // plenty of experience: capped at max level 5 - current 1 = 4, remainder left 0
    g_statexp[3] = 100000; rem = -1.0f;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 4 && rem == 0.0f, "capped at the maximum level");
    // already at the maximum
    g_trainable[2] = 5;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 0, "at the maximum");
    // a level above the maximum (7 > 5): the cap returns max - level, negative
    g_trainable[2] = 7;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == -2, "above the maximum: max - level");
    // empty track: max level clamps to 0; level 0 -> 0
    g_trainable[1] = 0;
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)1, rem) == 0, "empty track");
    // the pool counts: category alone 100 < 200, with the pool 150 more -> one level
    g_trainable[2] = 1; g_statexp[3] = 100; W32(g_hero, 0x14, 150);
    check(hero->GetTrainableStatPossibleLevelUpAmount((EHeroTrainableStatType)2, rem) == 1 && rem == 50.0f / 300.0f, "spendable pool added");
    if (!g_fail) printf("006d6fa0_TEST PASS\n");
    return g_fail;
}