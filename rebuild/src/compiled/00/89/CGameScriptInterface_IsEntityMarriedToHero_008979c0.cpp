// CGameScriptInterface::IsEntityMarriedToHero(const CScriptThing& thing) const -- a live creature whose wife component (interface 0xd7) answers IsMarriedToThePlayer()
// Leaf: ?IsEntityMarriedToHero@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008979C0, 101 bytes, `ret 4`; Ego_r.pdb ?IsEntityMarriedToHero@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z.
// Body: t = thing.GetPThing() (slot 11, +0x2c); if t, not dead (bit 0 of +0x91), a creature (type byte +0x10 == 1) and
// holding interface 0xd7 (bit 23 of UsingComponents word 6, +0x38), the component from the const Components map
// (LowerBound 0x004365B0, key in the dead `thing` slot) answers IsMarriedToThePlayer() (0x006CEC80, compares a +0x20 field against a 0x0049D870 value); else false.
// The byte twin of 0x00897A30 IsEntityMarriable (only the callee differs).
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0xd7 is modelled as CTCWife (Ego_r.pdb
// ?IsMarriedToThePlayer@CTCWife@@QBE_NXZ / ?IsWillingToMarry@CTCWife@@QBE_NXZ); the interface number is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCWife {                                         // interface 0xd7
public:
    bool IsMarriedToThePlayer() const;                   // 0x006CEC80
    bool IsWillingToMarry() const;                       // 0x006CFEC0
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityMarriedToHero(const CScriptThing& thing) const
{
    const CThing* t = thing.GetPThing();
    if (t && !t->IsDead() && t->GetType() == 1)
    {
        const const CTCWife* wife;
        if (t->PeekTC(TCI_WIFE, &wife) && wife->IsMarriedToThePlayer())
            return true;
    }
    return false;
}