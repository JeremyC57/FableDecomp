#ifndef FABLE_ENGINE_CHEROABILITYDEF_H
#define FABLE_ENGINE_CHEROABILITYDEF_H
// Complete Ego_r member order, with retail parent size0x3c and twelve-byte STLport vectors.
// Retail005eeff2 reads Ability+0x3c, IconGraphicIndex+0x44 and LevDescription+0x60.
#include "engine/CParentDefClassBase.h"
#include "engine/EHeroAbility.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
class CHeroAbilityDef : public CParentDefClassBase { public:
    EHeroAbility Ability;
    EHeroExperienceStatCategory Stat;
    long IconGraphicIndex,IconEffectIndex,IconActiveEffectIndex;
    C2DVector IconEffectOffset;
    long Name,Description;
    std::vector<long> LevDescription;
    long MaxedOutDescription;
    std::vector<long> StaminaCost;
    long StatExperienceGainedOnUse;
    std::vector<long> ExperienceCostsToUpgrade;
    long ABXYPriority;
    float MoralityCostFactor;
    bool Aggressive;
    unsigned int DummyObject[1]; // Opaque CDefIndex member, preserved at+0x98.
};
FABLE_STATIC_ASSERT(sizeof(CHeroAbilityDef)==0x9c);
#endif
