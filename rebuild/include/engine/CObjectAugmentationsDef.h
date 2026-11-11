#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES
#ifndef FABLE_ENGINE_COBJECTAUGMENTATIONSDEF_H
#define FABLE_ENGINE_COBJECTAUGMENTATIONSDEF_H
#include "engine/CSubDefClassBase.h"
#include "engine/CObjectAugmentationParticleSet.h"
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_map.h"
class CPersistContext;
// Layout: complete original Ego_r/FableWin member records; native STLport containers are twelve bytes.
class CObjectAugmentationsDef:public CSubDefClassBase{
public:
 virtual void Copy(const CDefClassBase * p0);
 virtual unsigned long GetSizeofClass();
 static CDefClassBase * FABLE_FASTCALL Alloc();
 static CCharString FABLE_FASTCALL StatGetTypeName();
 long NumberOfSlots;
 long MaximumNumberOfSlots;
 std::map<enum EObjectAugmentationType,CObjectAugmentationParticleSet,std::less<enum EObjectAugmentationType>,std::allocator<std::pair<enum EObjectAugmentationType const ,CObjectAugmentationParticleSet> > > AugmentationParticles;
 unsigned long EmptySlotName;
 std::vector<long,std::allocator<long> > InitialAugmentations;
 std::vector<long,std::allocator<long> > SlotUpgradeCosts;
 std::map<enum EObjectAugmentationType,CDefString,std::less<enum EObjectAugmentationType>,std::allocator<std::pair<enum EObjectAugmentationType const ,CDefString> > > InitSounds;
 std::map<enum EObjectAugmentationType,CDefString,std::less<enum EObjectAugmentationType>,std::allocator<std::pair<enum EObjectAugmentationType const ,CDefString> > > LoopingSounds;
 long FireProjectileTrailEffect;
 long LightningProjectileTrailEffect;
 long FireAndLightningProjectileTrailEffect;
 long FireProjectileOnHitEffect;
 long LightningProjectileOnHitEffect;
 long FireAndLightningProjectileOnHitEffect;
 CCharString WeaponTrailAttachmentPoint;
 virtual void Transfer(CPersistContext & p0);
 CObjectAugmentationsDef(const CObjectAugmentationsDef & p0);
 CObjectAugmentationsDef();
 virtual ~CObjectAugmentationsDef();
 CObjectAugmentationsDef & operator=(const CObjectAugmentationsDef & p0);
};
FABLE_STATIC_ASSERT(sizeof(CObjectAugmentationsDef)==140);
#endif
#else
#ifndef FABLE_ENGINE_COBJECTAUGMENTATIONSDEF_H
#define FABLE_ENGINE_COBJECTAUGMENTATIONSDEF_H
// CObjectAugmentationsDef (new retail-model header, chunk 1002): RTTI .?AVCObjectAugmentationsDef@@ : CSubDefClassBase : CDefClassBase
// (ghidra_out/rtti_bases.tsv). Members: Ego_r.pdb CObjectAugmentationsDef (struct_layouts_egor.tsv, 0xA0 there) in order and spelling
// up to InitialAugmentations at the retail offsets: the STLport map AugmentationParticles is 12 bytes (Ego_r's 16), so EmptySlotName
// is +0x3C and InitialAugmentations +0x40 (Ego_r +0x44). Retail evidence: CTCInventoryWeapons description rows 0x005C71AA / 0x005C6B26
// read NumberOfSlots `[esi+28h]` and InitialAugmentations `[esi+40h]` / `[esi+44h]` after GetSubDef<CObjectAugmentationsDef>
// 0x0042AF9E (whose class-name literal is 'CObjectAugmentationsDef'). Later members are not modelled. Requires FABLE_RETAIL_DEF_CLASS_BASE.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CObjectAugmentationsDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "stlport/fable_containers.h"
class CObjectAugmentationsDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CObjectAugmentationsDef", -1); }   // inline (Ego_r name set)
    unsigned char _pad_0x25[0x3];                                    // +0x25
    long          NumberOfSlots;                                     // +0x28
    long          MaximumNumberOfSlots;                              // +0x2c
    unsigned char AugmentationParticles[0xc];                        // +0x30 map<EObjectAugmentationType,CObjectAugmentationParticleSet>
    unsigned long EmptySlotName;                                     // +0x3c
    std::vector<long> InitialAugmentations;                          // +0x40
};
FABLE_STATIC_ASSERT(offsetof(CObjectAugmentationsDef, NumberOfSlots) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CObjectAugmentationsDef, InitialAugmentations) == 0x40);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_COBJECTAUGMENTATIONSDEF_H

#endif
