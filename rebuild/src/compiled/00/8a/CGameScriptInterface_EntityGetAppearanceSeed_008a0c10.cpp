// CGameScriptInterface::EntityGetAppearanceSeed(const CScriptThing& thing, unsigned long& seed) const -- when the thing exists and has a CTCRandomAppearanceMorph (interface 0x03), read its appearance seed into the out argument
// Leaf: ?EntityGetAppearanceSeed@CGameScriptInterface@@UBEXABVCScriptThing@@AAK@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A0C10, thiscall, virtual.
// Identity (proven): vtable 0x01260F0C (RTTI .?AVCGameScriptInterface@@) slot 483; ghidra_out/cgsi_slots_aligned.tsv (Ego_r.pdb
//   vtable order) names it ?EntityGetAppearanceSeed@CGameScriptInterface@@UBEXABVCScriptThing@@AAK@Z.
// Guard: the script thing's thing (slot 11) is non-null; the component is CTCRandomAppearanceMorph (interface 0x3: GetInterfaceType of its RTTI vtable, or its Ego_r.pdb method).
// Callees: CScriptThing slots 75/11, CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> >::LowerBound(const ETCInterfaceType&) 0x0040F020 (the inlined CThing::GetInterface), none (the inline CTCRandomAppearanceMorph::GetAppearanceSeed).
// Shape: the landed L083/L084 GetInterface model (2026-10-27_linux_hand955); the lookup key is homed in the dead thing-argument slot.
// Layout: local models -- engine/CGameScriptInterface.h carries its vptr as a data member and the CThing/CScriptThing/TC headers are flat
//   Ego_r layouts (Dinkumware offsets); component offsets are retail (Ego_r minus 4 per STLport container before the member).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCRandomAppearanceMorph {
public:
    unsigned char _pad00[0x18];
    unsigned long Seed;                                   // +0x18 (Ego_r)
    unsigned long GetAppearanceSeed() const { return Seed; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityGetAppearanceSeed(const CScriptThing& thing, unsigned long& seed) const
{
    CThing* t = thing.GetPThing();
    CTCRandomAppearanceMorph* morph;
    if (t && t->GetTC(TCI_APPEARANCE_MORPH, &morph))
        seed = morph->GetAppearanceSeed();
}