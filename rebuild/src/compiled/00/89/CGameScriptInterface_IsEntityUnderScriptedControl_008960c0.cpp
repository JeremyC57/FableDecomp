// CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing& entity) const -- script call: whether a live entity's scripted-control component (interface 0x1f) is locked
// Leaf: ?IsEntityUnderScriptedControl@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008960C0, 82 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 9 holds this body; slot 9 is
//   ?IsEntityUnderScriptedControl@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z (ghidra_out/cgsi_slots_aligned.tsv).
//   Interface 0x1f = CTCScriptedControl (its vtable 0x012658E4 slot 21 is 0x00719A40, which returns 0x1f). Getter:
//   FableWin.pdb ?IsLocked@CTCScriptedControl@@QBE_NXZ, inline (Locked +0x18, as the landed CTCScriptedControl rows).
// Body: t = entity.GetPThing() (slot 11); live t holding interface 0x1f (the sign bit of UsingComponents word 0, `jns`):
//   PeekTC (const LowerBound 0x004365B0) and return IsLocked().
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCScriptedControl {                              // interface 0x1f
public:
    unsigned char _pad00[0x18];
    bool Locked;                                         // +0x18
    bool IsLocked() const { return Locked; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing& entity) const
{
    const CThing* t = entity.GetPThing();
    if (t && !t->IsDead())
    {
        const const CTCScriptedControl* tc;
        if (t->PeekTC(TCI_SCRIPTED_CONTROL, &tc))
            return tc->IsLocked();
    }
    return false;
}