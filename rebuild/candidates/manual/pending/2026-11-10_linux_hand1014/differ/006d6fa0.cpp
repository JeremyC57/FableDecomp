// CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat, float& fractionTowardsNext) -- how many levels the stat could be raised with its category's experience plus the spendable pool, and how far the remainder goes towards the level after
// Grade: DIFFER(228v220) -- c2 hoists TrainableStatLevels[stat] out of the level loop and strength-reduces (level+n)*72; retail re-reads the level per pass and keeps stat in ebp (probe with the subtraction through a by-reference inline helper reached 225v220)
// Leaf: ?GetTrainableStatPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroTrainableStatType@@AAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D6FA0, 220 bytes, __thiscall, `ret 8`. Ego_r.pdb
//   ?GetTrainableStatPossibleLevelUpAmount@CTCHeroExperience@@QAEJW4EHeroTrainableStatType@@AAM@Z.
// Callees: none (GetCategoryTrainableStatIsIn 0x006D6850 and GetTrainableStatMaxLevel 0x006D6770 are same-TU bodies, expanded).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CTCHEROEXPERIENCE_CATEGORY_BODY
#define FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS
#define FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"

long CTCHeroExperience::GetTrainableStatPossibleLevelUpAmount(EHeroTrainableStatType stat, float& fractionTowardsNext)
{
    fractionTowardsNext = 0.0f;
    long experience = StatExperiencePoints[GetCategoryTrainableStatIsIn(stat)] + ExperiencePointsAvailableToSpend;
    for (long levels = 1; ; ++levels) {
        if (GetTrainableStatLevel(stat) + levels > GetTrainableStatMaxLevel(stat))
            return GetTrainableStatMaxLevel(stat) - GetTrainableStatLevel(stat);
        long cost = PHeroStatsDef->StatLevels[stat].Abilities[GetTrainableStatLevel(stat) + levels].ExperienceCost;
        if (experience < cost) {
            fractionTowardsNext = (float)experience / cost;
            return levels - 1;
        }
        experience -= cost;
    }
}
