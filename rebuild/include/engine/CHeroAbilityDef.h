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
#ifdef FABLE_RETAIL_CHEROABILITYDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CHeroAbilityDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CHeroAbilityDef)==0x9c);
#endif
