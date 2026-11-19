// CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat_type, float& remainder) -- how many levels the stat could be raised with its category's experience plus the spendable pool (capped at the stat's maximum), and how far the rest goes towards the level after
// Leaf: ?GetTrainableStatPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroTrainableStatType@@AAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D6FA0, 220 bytes, thiscall `ret 8`. Ego_r.pdb / FableWin.pdb (tc_hero_experience.obj) same name.
// Shape: FableWin 0x01EF0C00 (/Od) statement order with Ego_d's locals (general_exp, stat_exp, LevelUp; NextLevelCost in the loop block):
//   general_exp = GetExperiencePointsAvailableToSpend(); remainder = 0; stat_exp = GetStatExperience(GetCategoryTrainableStatIsIn(stat_type))
//   + general_exp; a `while (true)` loop (FableWin `mov edx,1 ; test edx,edx ; je`) over LevelUp with the max-level return, the
//   NextLevelCost test (`break` after the remainder) and `else stat_exp -= NextLevelCost;`, then `LevelUp--; return LevelUp;`.
//   TrainableStatLevels[stat_type] is read directly (FableWin calls vector<long>::operator[] there). The same-TU accessors
//   GetCategoryTrainableStatIsIn (0x006D6850), GetTrainableStatMaxLevel (0x006D6770) and GetStatExperience (0x006D6510, new opt-in
//   FABLE_RETAIL_CTCHEROEXPERIENCE_GETSTATEXPERIENCE_BODY) are expanded, as retail does.
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