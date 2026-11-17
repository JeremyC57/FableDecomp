#ifndef FABLE_ENGINE_CARMOURDEF_H
#define FABLE_ENGINE_CARMOURDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE
#include "engine/EObjectAugmentationType.h"
#endif
class CArmourDef:public CParentDefClassBase {
#ifdef FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE
// NF67 opt-in (round 7, row 0x00869020 CHitParameters::SetAsModifiedByArmour, for the visible same-TU body of the anonymous-namespace
// GetArmourResponseAugmentationDamageMultiplier 0x00868A80): AugmentationResponse as its real member type, Ego_r.pdb CArmourDef
// std::map<EObjectAugmentationType,float> (a 12-byte STLport map at retail +0x3C, like DamageTypeResponse at +0x48; 0x00868A80 walks
// it in place: header [def+3Ch], key at node +0x10, value at +0x14). Same layout (asserts below).
public:std::map<EObjectAugmentationType,float> AugmentationResponse;std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
#else
public:unsigned char AugmentationResponse[12];std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
#endif
#ifdef FABLE_RETAIL_CARMOURDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CArmourDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(9, offsetof(CArmourDef,DamageTypeResponse)==0x48);
FABLE_STATIC_ASSERT_AT(10, sizeof(CArmourDef)==0x6c);
#endif
