// CTCHeroExperience::IsToAutoBlock() const -- true when the hero's weighted skill and strength training levels both exceed the combat definition's auto-block thresholds
// Leaf: ?IsToAutoBlock@CTCHeroExperience@@QBE_NXZ
// Retail: 0x006d6cb0,495 bytes,const thiscall,no arguments.
// Flags:
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_BLOCK_THRESHOLDS
#define FABLE_RETAIL_CHEROSTATSDEF_STATLEVELS
#define FABLE_RETAIL_CHEROCOMBATDEF_BLOCK_THRESHOLDS
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
bool CTCHeroExperience::IsToAutoBlock() const
{
    CDefPointer<const CHeroCombatDef> combat = PHeroCombatDef;
    float f3;
    {
        long level=GetTrainableStatLevel(HERO_STAT_SKILL_SPEED);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_SKILL_SPEED);
        float value=(float)level;
        f3=value/maxLevel;
    }
    float f4;
    {
        long level=GetTrainableStatLevel(HERO_STAT_SKILL_ACCURACY);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_SKILL_ACCURACY);
        float value=(float)level;
        f4=value/maxLevel;
    }
    float f5;
    {
        long level=GetTrainableStatLevel(HERO_STAT_SKILL_STEALTH);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_SKILL_STEALTH);
        float value=(float)level;
        f5=value/maxLevel;
    }
    float skill = f3 * 0.3f + f4 * 0.3f + f5 * 0.4f;
    float f1;
    {
        long level=GetTrainableStatLevel(HERO_STAT_STRENGTH_HEALTH);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_STRENGTH_HEALTH);
        float value=(float)level;
        f1=value/maxLevel;
    }
    float f0;
    {
        long level=GetTrainableStatLevel(HERO_STAT_STRENGTH_PHYSIQUE);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_STRENGTH_PHYSIQUE);
        float value=(float)level;
        f0=value/maxLevel;
    }
    float f2;
    {
        long level=GetTrainableStatLevel(HERO_STAT_STRENGTH_TOUGHNESS);
        long maxLevel=GetTrainableStatMaxLevel(HERO_STAT_STRENGTH_TOUGHNESS);
        float value=(float)level;
        f2=value/maxLevel;
    }
    float strength = (f0 + f1) * 0.3f + f2 * 0.4f;
    if (skill > combat->AutoBlockSkillThreshold && strength > combat->AutoBlockStrengthThreshold)
        return true;
    return false;
}