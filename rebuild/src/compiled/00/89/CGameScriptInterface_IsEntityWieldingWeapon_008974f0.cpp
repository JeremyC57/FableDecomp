// CGameScriptInterface::IsEntityWieldingWeapon(const CScriptThing& thing) const -- a live thing whose carrying component (interface 0x46) has a primary-slot thing holding interface 0x48 (the weapon role)
// Leaf: ?IsEntityWieldingWeapon@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008974F0, 105 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: t = thing.GetPThing() (slot 11); false unless t, not dead (bit 0 of +0x91) and holding interface 0x46 (bit 6 of
// UsingComponents word 2, +0x28). carrying = PeekTC (const LowerBound 0x004365B0, key in the dead `thing` slot);
// w = carrying->GetThingInPrimarySlot() (0x006A1440, ?GetThingInPrimarySlot@CTCCarrying@@QBEPAVCThing@@XZ, engine_api.tsv
// + bsim 1.0); result = w && w holds interface 0x48 (bit 8 of word 2) -- the && is int-typed (`mov eax,1` / `xor eax,eax`)
// while the early-out returns are `xor al,al`.
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0x46 = CTCCarrying (from the callee's class);
// interface 0x48 as a weapon role is not confirmed.
// Shape: /O2 (16-aligned entry); the PeekTC out-pointer form. The early-return form (`if (!t || dead) return false;
// if (!PeekTC) return false;`) keeps the component in eax and moves `pop esi` before the call; nesting the positive branch matches.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCCarrying {                                     // interface 0x46
public:
    CThing* GetThingInPrimarySlot() const;               // 0x006A1440
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsEntityWieldingWeapon(const CScriptThing& thing) const
{
    const CThing* t = thing.GetPThing();
    if (t && !t->IsDead())
    {
        const const CTCCarrying* carrying;
        if (t->PeekTC(TCI_CARRYING, &carrying))
        {
            const CThing* w = carrying->GetThingInPrimarySlot();
            return w && w->HasTC(TCI_WEAPON);
        }
    }
    return false;
}