// CGameScriptInterface::IsEntityStealable(const CScriptThing& thing) const -- a valid thing whose interface-0x54 component has its +0xd flag set
// Leaf: ?IsEntityStealable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089A330, 94 bytes, `ret 4`; Ego_r.pdb ?IsEntityStealable@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11, +0x2c); if t holds interface 0x54 (bit 20 of
// UsingComponents word 2, +0x28) the component from the const Components map (LowerBound 0x004365B0, key in the dead
// `thing` slot) answers its +0xd byte; otherwise false.
// Layout: CThing as the matched 0x0089B0B0 / 0x00897AA0 rows. The interface-0x54 component class and the +0xd member name
// are not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCInterface54 {                                  // interface 0x54 (class not confirmed)
public:
    unsigned char _pad00[0xd];
    bool          Stealable;                             // +0x0d (name not confirmed)
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityStealable(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        const CThing* t = thing.GetPThing();
        const const CTCInterface54* tc;
        if (t->PeekTC(TCI_STOCK_ITEM, &tc) && tc->Stealable)
            return true;
    }
    return false;
}