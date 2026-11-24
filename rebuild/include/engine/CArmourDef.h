#ifndef FABLE_ENGINE_CARMOURDEF_H
#define FABLE_ENGINE_CARMOURDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
#ifdef FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE
#include "engine/EObjectAugmentationType.h"
#endif
#ifdef FABLE_RETAIL_CARMOURDEF_DTOR
#include "engine/EObjectAugmentationType.h"
#endif
#if defined(FABLE_RETAIL_CARMOURDEF_DTOR) && defined(_MSC_VER)
// NF84 (with the opt-in below; the engine/CCardPositionsDef.h FABLE_RETAIL_CCARDPOSITIONSDEF_DTOR_NOVTABLE precedent): retail's out-of-line
// destructor 0x004585BD stores no vtable of its own (`push esi ; mov esi,ecx ; lea ecx,[esi+54h]` ... `jmp ~CParentDefClassBase`);
// VC7.1 omits that store under __declspec(novtable). Only the destructor's TU may use it. Layout unchanged.
class __declspec(novtable) CArmourDef:public CParentDefClassBase {
#else
class CArmourDef:public CParentDefClassBase {
#endif
#ifdef FABLE_RETAIL_CARMOURDEF_DTOR
// NF84 opt-in (round 7, q11044 binder, row 0x004585BD itself): the destructor ??1CArmourDef@@UAE@XZ (Ego_r.pdb 0x00465A8E, FableWin.pdb
// 0x0194A5A8; retail 0x004585BD, called by the scalar deleting destructor 0x004585A1) and the three maps it destroys as their real
// member types: AugmentationResponse map<EObjectAugmentationType,float> (+0x3C, tree dtor 0x00451A85), DamageTypeResponse and
// ArmourThresholds map<EDamageAttribute,float> (+0x48 / +0x54, tree dtor 0x00451AC6) (Ego_r.pdb CArmourDef, 16-byte Dinkumware maps
// at 64 / 80 / 96 = retail 12-byte STLport maps at +0x3C / +0x48 / +0x54). Same layout (asserts below).
public:std::map<EObjectAugmentationType,float> AugmentationResponse;std::map<EDamageAttribute,float> DamageTypeResponse;std::map<EDamageAttribute,float> ArmourThresholds;bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
    virtual ~CArmourDef();                                               // 0x004585BD
#else
#ifdef FABLE_RETAIL_CARMOURDEF_AUGMENTATIONRESPONSE
// NF67 opt-in (round 7, row 0x00869020 CHitParameters::SetAsModifiedByArmour, for the visible same-TU body of the anonymous-namespace
// GetArmourResponseAugmentationDamageMultiplier 0x00868A80): AugmentationResponse as its real member type, Ego_r.pdb CArmourDef
// std::map<EObjectAugmentationType,float> (a 12-byte STLport map at retail +0x3C, like DamageTypeResponse at +0x48; 0x00868A80 walks
// it in place: header [def+3Ch], key at node +0x10, value at +0x14). Same layout (asserts below).
public:std::map<EObjectAugmentationType,float> AugmentationResponse;std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
#else
public:unsigned char AugmentationResponse[12];std::map<EDamageAttribute,float> DamageTypeResponse;unsigned char ArmourThresholds[12];bool AllHitsKnockdown,AllHitsNegated,AllHitsCauseRecoil;long DamageMaterial,BlockMaterial;
#endif
#endif
#ifdef FABLE_RETAIL_CARMOURDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CArmourDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
#ifdef FABLE_RETAIL_CARMOURDEF_COPY_DECLS
    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for CArmourDef::Copy 0x0046133F, which tail-calls the out-of-line
    // operator= (retail `jmp` 0x00461344). ?Copy@CArmourDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at 0x0046133F (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name
    // control fails); ??4CArmourDef@@QAEAAV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb) is the operator= at 0x00461344.
public:
    virtual void Copy(const CDefClassBase*);
    CArmourDef& operator=(const CArmourDef&);
#endif
};
FABLE_STATIC_ASSERT_AT(9, offsetof(CArmourDef,DamageTypeResponse)==0x48);
FABLE_STATIC_ASSERT_AT(10, sizeof(CArmourDef)==0x6c);
#endif
