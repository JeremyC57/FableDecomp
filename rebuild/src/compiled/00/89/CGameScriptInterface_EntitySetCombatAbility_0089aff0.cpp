// CGameScriptInterface::EntitySetCombatAbility(const CScriptThing& thing, float ability) const -- for a live thing with interface 0x30, forward the ability to that component
// Leaf: ?EntitySetCombatAbility@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089AFF0, 90 bytes, `ret 8`; Ego_r.pdb ?EntitySetCombatAbility@CGameScriptInterface@@UBEXABVCScriptThing@@M@Z.
// Body: t = thing.GetPThing() (slot 11, +0x2c); if t, not dead (bit 0 of +0x91) and holding interface 0x30 (bit 16 of
// UsingComponents +0x24), the component from the Components map (LowerBound 0x0040F020, key in the dead `thing` slot)
// gets 0x00734210(ability, true). No IsValid call.
// Layout: CThing as the matched 0x0089B0B0 / 0x0089D8B0 rows. The interface-0x30 component class and the callee name
// (0x00734210, walks a std::map at +0x70) are not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCInterface30 {                                  // interface 0x30 (class not confirmed)
public:
    void SetCombatAbility(float ability, bool flag);     // 0x00734210 (name not confirmed)
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetCombatAbility(const CScriptThing& thing, float ability) const
{
    CThing* t = thing.GetPThing();
    if (t && !t->IsDead())
    {
        CTCInterface30* tc;
        if (t->GetTC(TCI_COMBAT, &tc))
            tc->SetCombatAbility(ability, true);
    }
}