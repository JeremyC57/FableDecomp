// CGameScriptInterface::SetCreatureGeneratorEnabled(const CScriptThing& thing, bool enabled) const -- when the script thing is valid and its thing has a CTCCreatureGenerator component (interface type 0x3D), call CTCCreatureGenerator::SetAsActive(enabled)
// Leaf: ?SetCreatureGeneratorEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089ED20, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 130.
// Identity: CGameScriptInterface::SetCreatureGeneratorEnabled(const CScriptThing&, bool) const -- proven; evidence: FableWin.pdb name set holds
//   ?SetCreatureGeneratorEnabled@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z (the catalog label agrees); it calls CTCCreatureGenerator::SetAsActive
//   0x00749DD0 (FableWin.pdb ?SetAsActive@CTCCreatureGenerator@@QAEX_N@Z; landed) on the interface-type-0x3D component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCCreatureGenerator::SetAsActive(bool) 0x00749DD0.
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
class CTCCreatureGenerator {
public:
    void SetAsActive(bool enabled);                              // 0x00749DD0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetCreatureGeneratorEnabled(const CScriptThing& thing, bool enabled) const
{
    if (thing.IsAlive()) {
        CThing* pThing = thing.GetPThing();
        CTCCreatureGenerator* component;
        if (pThing->GetTC(TCI_CREATURE_GENERATOR, &component))
            component->SetAsActive(enabled);
    }
}