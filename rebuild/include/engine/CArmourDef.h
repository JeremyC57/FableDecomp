#ifndef FABLE_ENGINE_CARMOURDEF_H
#define FABLE_ENGINE_CARMOURDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
class CArmourDef:public CParentDefClassBase {
public:unsigned char AugmentationResponse[12];std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
#ifdef FABLE_RETAIL_CARMOURDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CArmourDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(9, offsetof(CArmourDef,DamageTypeResponse)==0x48);
FABLE_STATIC_ASSERT_AT(10, sizeof(CArmourDef)==0x6c);
#endif
