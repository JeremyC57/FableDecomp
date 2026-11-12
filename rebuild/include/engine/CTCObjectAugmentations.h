#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES
#ifndef FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
#define FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CObjectAugmentationsDef.h"
#include "engine/CObjectAugmentationSoundSet.h"
#include "engine/ParticleAndEndPoints.h"
#include "engine/AttackParticleAndEndPoints.h"
class CThingBaseDef;
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
class CTCObjectAugmentations:public CTCBase{
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCObjectAugmentations(const CTCObjectAugmentations & p0);
 CTCObjectAugmentations(CThing & p0);
 virtual ~CTCObjectAugmentations();
 virtual bool IsComponentSerialisable() const;
 virtual void OnSerialise(CPersistContext & p0, bool p1);
 virtual void OnInitialActivate();
 virtual void OnSetInLimbo(bool p0);
 virtual bool HasFrameUpdate() const;
 virtual void FrameUpdate();
 void AttachAugmentationToSlot(long p0, long p1);
 void AttachAugmentationToSlot(CThing & p0, long p1);
 void RemoveAugmentationFromSlot(long p0);
 long GetFreeSlot() const;
 void GetAugmentations(std::vector<long,std::allocator<long> > & p0) const;
 long GetNumberOfSlots() const;
 long GetMaximumNumberOfSlots() const;
 long GetCostToUpgradeNumberOfSlots() const;
 void AddNewSlot();
 CWideString GetAugmentationNameInSlot(long p0) const;
 void CopyAugmentationDataFromThing(const CThing & p0);
 bool HasAugmentationOfType(EObjectAugmentationType p0) const;
 bool HasCombinationOfAugmentations(const std::vector<enum EObjectAugmentationType,std::allocator<enum EObjectAugmentationType> > & p0, EObjectAugmentationType p1) const;
 bool HasCombinationOfAugmentations(EObjectAugmentationType p0) const;
 float GetDamageMultiplier() const;
 long GetHealthToAddThisFrame() const;
 long GetStaminaToAddThisFrame() const;
 float GetExperienceMultiplier() const;
 bool HasAugmentationInSlot(long p0) const;
 CDefPointer<CThingBaseDef const > GetAugmentationInSlot(long p0) const;
 void FireCombatAugEffect();
 void StopCombatAugEffect();
 long GetOnHitExplodingEffect() const;
 void StopTrailEffect();
 static float FABLE_FASTCALL GetDamageMultiplierFromAugmentationSet(const std::vector<long,std::allocator<long> > & p0);
 static CWideString FABLE_FASTCALL GetAugmentationName(long p0);
private:
 void AttachAugmentationParticles();
 void SetupAugmentationSounds();
 void ClearAllParticleEffects();
 void ClearAllSounds();
 void AttemptToStartParticle(CIntelligentPointer<CThing> & p0, long p1, long p2, const CCharString & p3, const CCharString & p4);
 void UpdateParticleAttachment(CIntelligentPointer<CThing> p0, const CCharString & p1, const CCharString & p2);
 CDefPointer<CObjectAugmentationsDef const > PDef;
 bool LoadedFromSaveGame;
 std::vector<CDefPointer<CThingBaseDef const >,std::allocator<CDefPointer<CThingBaseDef const > > > Augmentations;
 std::map<enum EObjectAugmentationType,CObjectAugmentationSoundSet,std::less<enum EObjectAugmentationType>,std::allocator<std::pair<enum EObjectAugmentationType const ,CObjectAugmentationSoundSet> > > InitSounds;
 std::map<enum EObjectAugmentationType,CObjectAugmentationSoundSet,std::less<enum EObjectAugmentationType>,std::allocator<std::pair<enum EObjectAugmentationType const ,CObjectAugmentationSoundSet> > > LoopingSounds;
 std::vector<std::vector<ParticleAndEndPoints,std::allocator<ParticleAndEndPoints> >,std::allocator<std::vector<ParticleAndEndPoints,std::allocator<ParticleAndEndPoints> > > > ParticleEffects;
 std::vector<std::vector<AttackParticleAndEndPoints,std::allocator<AttackParticleAndEndPoints> >,std::allocator<std::vector<AttackParticleAndEndPoints,std::allocator<AttackParticleAndEndPoints> > > > AttackParticleEffects;
 std::vector<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > CurrentRunningAugEffect;
 long CurrentRunningAugEffectIndex;
 long CurPeriod;
 bool InStrikeAction;
 CIntelligentPointer<CThing> ProjectileTrailEffect;
public:
 CTCObjectAugmentations & operator=(const CTCObjectAugmentations & p0);
};
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
inline bool CTCObjectAugmentations::HasAugmentationOfType(EObjectAugmentationType type)const{
 for(long c1=0;c1<Augmentations.size();++c1){
  if(Augmentations[c1].Get()){
   CDefPointer<const CAugmentationDef>paugmentation_def;
   Augmentations[c1]->GetSubDef(&paugmentation_def);
   if(paugmentation_def->Type==type)return true;
  }
 }
 return false;
}
inline bool CTCObjectAugmentations::HasCombinationOfAugmentations(EObjectAugmentationType types)const{
 unsigned long result=0;
 for(long c1=0;c1<Augmentations.size();++c1){
  if(Augmentations[c1].Get()){
   CDefPointer<const CAugmentationDef>paugmentation_def;
   Augmentations[c1]->GetSubDef(&paugmentation_def);
   result|=paugmentation_def->Type;
  }
 }
 return (result&types)==types;
}
FABLE_STATIC_ASSERT(sizeof(CTCObjectAugmentations)==112);
// Inline-budget: native 00767b8c calls this exact original destructor at 0076b0e0.
// Explicit instantiation adds no new specialization or function body.
// Inline-budget: native periodic-vector destruction expands this exact generic range body.
#ifdef _MSC_VER
template FABLE_FORCEINLINE void FABLE_FASTCALL std::__destroy_aux<ParticleAndEndPoints*>(ParticleAndEndPoints*,ParticleAndEndPoints*,const std::__false_type&);
template __declspec(noinline) std::vector<ParticleAndEndPoints>::~vector();
#else
template std::vector<ParticleAndEndPoints>::~vector();
#endif

#endif
#else
#ifndef FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
#define FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H
// New shared header (chunk 1002, CTCInventoryWeapons rows 0x005C4671 / 0x005CB2B0): the retail CTCObjectAugmentations over
// FABLE_RETAIL_CTCBASE (RTTI .?AVCTCObjectAugmentations@@ : CTCBase). Members are not modelled; only what the rows call, in the
// Ego_r.pdb / FableWin.pdb spellings: ?GetFreeSlot@CTCObjectAugmentations@@QBEJXZ (retail 0x00765D50),
// ?AttachAugmentationToSlot@CTCObjectAugmentations@@QAEXJJ@Z (0x00768120), the static __fastcall ?GetDamageMultiplierFromAugmentationSet@CTCObjectAugmentations@@SIMABV?$vector@JV?$allocator@J@std@@@std@@@Z
// (0x007661C0) and ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z (0x00765F90). StatGetInterfaceType: TCI_OBJECT_AUGMENTATIONS (191;
// FableWin ??$PeekTC@$$CBVCTCObjectAugmentations@@@CThing@@..., retail tests bit 0xBF before its component-map lookup).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
#include "engine/CDefPointer.h"
class CObjectAugmentationsDef;
class CThingBaseDef;
#endif
class CTCObjectAugmentations : public CTCBase {
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
long GetOnHitExplodingEffect()const;void StopTrailEffect();
#endif
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_OBJECT_AUGMENTATIONS; }
    long GetFreeSlot() const;                                        // 0x00765D50
    void AttachAugmentationToSlot(long augmentationDefIndex, long slot);   // 0x00768120
    static float __fastcall GetDamageMultiplierFromAugmentationSet(const std::vector<long>& augmentations);   // 0x007661C0
    static CWideString __fastcall GetAugmentationName(long augmentationDefIndex);   // 0x00765F90 ?GetAugmentationName@CTCObjectAugmentations@@SI?AVCWideString@@J@Z
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_SLOTS
    // Retail opt-in (row 0x005BE8F3 NInventory::CTCInventoryBase::CreateInventoryObject): the slot accessors, Ego_r.pdb / FableWin.pdb
    // ?GetNumberOfSlots@CTCObjectAugmentations@@QBEJXZ (0x00765D80), ?GetMaximumNumberOfSlots@CTCObjectAugmentations@@QBEJXZ (0x00765D90),
    // ?AddNewSlot@CTCObjectAugmentations@@QAEXXZ (0x00766D70), ?RemoveAugmentationFromSlot@CTCObjectAugmentations@@QAEXJ@Z (0x007681D0);
    // all out of line in retail (landed rows). Declarations only: no layout change.
    long GetNumberOfSlots() const;                                   // 0x00765D80
    long GetMaximumNumberOfSlots() const;                            // 0x00765D90
    void AddNewSlot();                                               // 0x00766D70
    void RemoveAugmentationFromSlot(long slot);                      // 0x007681D0
#endif
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_GETAUGMENTATIONS
    // Retail opt-in (row 0x00868CE0 CHitParameters::SetWeapon): Ego_r.pdb
    // ?GetAugmentations@CTCObjectAugmentations@@QBEXAAV?$vector@JV?$allocator@J@std@@@std@@@Z, out of line in retail at 0x00766920 (landed).
    // Declaration only.
    void GetAugmentations(std::vector<long>& augmentations) const;   // 0x00766920
#endif
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_DAMAGEMULTIPLIER
    // Retail opt-in (agentNF10, CTCInventoryWeapons rows 0x005C7913 / 0x005C7B10 / 0x005C7D0D / 0x005C7F07): Ego_r.pdb / FableWin.pdb
    // ?GetDamageMultiplier@CTCObjectAugmentations@@QBEMXZ, out of line in retail at 0x00766140 (landed). Declaration only.
    float GetDamageMultiplier() const;                               // 0x00766140
#endif
#ifdef FABLE_RETAIL_CTCOBJECTAUGMENTATIONS_MEMBERS
    // Retail opt-in (agentNF10, rows 0x00768010 OnInitialActivate / 0x00768120 AttachAugmentationToSlot): the first Ego_r.pdb members
    // (struct_layouts_egor.tsv; PDef +0x0C, LoadedFromSaveGame +0x10, Augmentations +0x14 are at the same offsets in retail: the
    // 12-byte STLport vector only moves the later maps) and the PDB spellings the rows define or call:
    // ?OnInitialActivate@CTCObjectAugmentations@@UAEXXZ (CTCBase slot 6), the private ?AttachAugmentationParticles@..@@AAEXXZ (0x007677E0)
    // and ?SetupAugmentationSounds@..@@AAEXXZ (0x00766C00). Retail reads LoadedFromSaveGame `[esi+10h]` and PDef `[esi+0Ch]`
    // (0x00768016..), walks Augmentations `[esi+14h]` / `[esi+18h]` (GetFreeSlot 0x00765D50). Requires FABLE_RETAIL_DEF_CLASS_BASE.
    virtual void OnInitialActivate();                                // slot 6 0x00768010
private:
    void AttachAugmentationParticles();                              // 0x007677E0
    void SetupAugmentationSounds();                                  // 0x00766C00
public:
    CDefPointer<const CObjectAugmentationsDef> PDef;                 // +0x0C
    bool LoadedFromSaveGame;                                         // +0x10
    std::vector<CDefPointer<const CThingBaseDef> > Augmentations;    // +0x14
#endif
};
#endif // FABLE_ENGINE_CTCOBJECTAUGMENTATIONS_H

#endif
