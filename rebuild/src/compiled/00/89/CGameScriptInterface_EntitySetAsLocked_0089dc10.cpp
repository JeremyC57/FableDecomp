// CGameScriptInterface::EntitySetAsLocked(const CScriptThing& thing, bool locked) const -- when the script thing is valid and its thing has a CTCActionUseScriptedHook component (interface type 0xC2), call CTCActionUseScriptedHook::SetEntranceAsLocked(locked)
// Leaf: ?EntitySetAsLocked@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089DC10, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 91.
// Identity: CGameScriptInterface::EntitySetAsLocked(const CScriptThing&, bool) const -- proven; evidence: FableWin.pdb name set holds
//   ?EntitySetAsLocked@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (the catalog label agrees); it calls CTCActionUseScriptedHook::SetEntranceAsLocked
//   0x00729C30 (FableWin.pdb ?SetEntranceAsLocked@CTCActionUseScriptedHook@@QAEX_N@Z; landed) on the interface-type-0xC2 component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCActionUseScriptedHook::SetEntranceAsLocked(bool) 0x00729C30.
// Shape: `this` unused; the GetInterface template is `__forceinline` and the lookup key lives in the dead `thing` argument
//   slot; the component is not null-tested; the value is forwarded as a dword.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCActionUseScriptedHook {
public:
    void SetEntranceAsLocked(bool locked);                              // 0x00729C30
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetAsLocked(const CScriptThing& thing, bool locked) const
{
    if (thing.IsAlive()) {
        CThing* pThing = thing.GetPThing();
        CTCActionUseScriptedHook* component;
        if (pThing->GetTC(TCI_ACTION_USE_SCRIPTED_HOOK, &component))
            component->SetEntranceAsLocked(locked);
    }
}