#ifndef FABLE_ENGINE_CHEROABILITYDEF_H
#define FABLE_ENGINE_CHEROABILITYDEF_H
// Complete Ego_r member order, with retail parent size0x3c and twelve-byte STLport vectors.
// Retail005eeff2 reads Ability+0x3c, IconGraphicIndex+0x44 and LevDescription+0x60.
#include "engine/CParentDefClassBase.h"
#include "engine/EHeroAbility.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CHEROABILITYDEF_TRANSFER
#include "engine/CDefIndex.h"
#endif
class CHeroAbilityDef : public CParentDefClassBase { public:
#ifdef FABLE_RETAIL_CHEROABILITYDEF_TRANSFER
// Opt-in (NF40, 2026-11-16, row 0x00469AB3 CHeroAbilityDef::Transfer): Ego_r.pdb / FableWin.pdb
// ?Transfer@CHeroAbilityDef@@UAEXAAVCPersistContext@@@Z, the def's override of CDefClassBase::Transfer (virtual slot 18), and the
// member DummyObject typed as the PDB's CDefIndex (engine/CDefIndex.h, 4 bytes: no layout change). Declaration only.
    virtual void Transfer(class CPersistContext&);
#endif
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
#ifdef FABLE_RETAIL_CHEROABILITYDEF_TRANSFER
    CDefIndex DummyObject;       // +0x98 (Ego_r.pdb CDefIndex DummyObject; same 4 bytes as the opaque form below)
#else
    unsigned int DummyObject[1]; // Opaque CDefIndex member, preserved at+0x98.
#endif
#ifdef FABLE_RETAIL_CHEROABILITYDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CHeroAbilityDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CHeroAbilityDef)==0x9c);
#endif
