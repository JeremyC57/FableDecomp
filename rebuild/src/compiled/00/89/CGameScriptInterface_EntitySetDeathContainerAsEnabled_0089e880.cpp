// CGameScriptInterface::EntitySetDeathContainerAsEnabled(const CScriptThing& thing, bool value) const -- if the script thing is valid and its thing has a CTCSearchableContainer (interface 0x3C), store value into its DeathContainerEnabled_unnamed flag
// Leaf: ?EntitySetDeathContainerAsEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089e880, 86 bytes, thiscall, `ret 8`.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 587; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb vtable order) names it ?EntitySetDeathContainerAsEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z; CTCSearchableContainer::GetInterfaceType() is 60 (0x3C, landed); the byte at retail +0x18 is the first CTCSearchableContainer member after the CTCContainer base (an unnamed Ego_r.pdb padding byte; the member name is descriptive).
// Callees: CScriptThing virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface: presence bit
//   test, lookup, end-or-larger-key -> end).
// Shape (family L083): the landed DisableFishingSpot 0x00895950; the GetInterface key is homed in the dead thing-argument slot.
// Header debt: engine/CScriptThing.h, CThing and the TC headers are flat layouts without vtables; modelled locally.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCSearchableContainer {
public:
    unsigned char _pad00[0x18];
    bool DeathContainerEnabled_unnamed;                                    // +0x18
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetDeathContainerAsEnabled(const CScriptThing& thing, bool value) const
{
    if (thing.IsAlive()) {
        CTCSearchableContainer* component;
        if (thing.GetPThing()->GetTC(TCI_CONTAINER, &component))
            component->DeathContainerEnabled_unnamed = value;
    }
}