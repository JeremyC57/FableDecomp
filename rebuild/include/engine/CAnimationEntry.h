#ifndef FABLE_ENGINE_CANIMATIONENTRY_H
#define FABLE_ENGINE_CANIMATIONENTRY_H
// Hand-written (chunk 873, row 0x005DD670 CAnimationEntry::StatRegisterClass): one animation of an animation bank. Ego_r.pdb layout
// (16 bytes): BankIndex +0x00, AnimName +0x04 / GroupName +0x08 (CDefString), Components +0x0c (CIVCountedPointer<CAnimComponentMap>).
// The three registries are static members (Ego_r.pdb `dynamic initializer for 'CAnimationEntry::Registered...'`):
//   RegisteredComponentsByToken 0x013B8C1C  map<CCharString, CIVCountedPointer<CAnimComponentBase> > (STLport _Rb_tree, 12 bytes);
//   RegisteredComponentsByType  0x013B8C28  CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > (0x10 bytes);
//   RegisteredComponentDefaults 0x013B8C38  the same type (reloc_bindings.tsv spells the two CVectorMaps).
// Needs FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_CCHARSTRING, FABLE_RETAIL_CDEFSTRINGTABLE.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CAnimComponentBase.h"
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
class CAnimComponentMap;
class CAnimationEntry {
public:
    // Ego_r.pdb / FableWin.pdb ?StatRegisterClass@CAnimationEntry@@SIXXZ (static __fastcall; retail 0x005DD670, one caller 0x0044E907).
    static void __fastcall StatRegisterClass();
    static std::map<CCharString, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentsByToken;          // 0x013B8C1C
    static CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentsByType;      // 0x013B8C28
    static CVectorMap<EAnimComponent, CIVCountedPointer<CAnimComponentBase> > RegisteredComponentDefaults;     // 0x013B8C38
#ifdef FABLE_RETAIL_CANIMATIONENTRY_ACCESSORS
    // Retail opt-in (chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): Ego_r.pdb / FableWin.pdb
    // ?GetAnimateThingFlags@CAnimationEntry@@QBEKXZ (retail 0x005DC390) and ?GetTransitionTime@CAnimationEntry@@QBEJXZ (0x005DC1D0),
    // both out of line. Declarations only.
    unsigned long GetAnimateThingFlags() const;                // 0x005DC390
    long GetTransitionTime() const;                            // 0x005DC1D0
#endif
    long                                   BankIndex;     // +0x00
    CDefString                             AnimName;      // +0x04
    CDefString                             GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap>   Components;    // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CAnimationEntry) == 0x10);
#endif // FABLE_ENGINE_CANIMATIONENTRY_H
