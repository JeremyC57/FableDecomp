// CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing& thing, long price) const -- when the script thing is valid and its thing has a CTCStockItem component (interface type 0x54), call CTCStockItem::SetPrice(price)
// Leaf: ?EntitySetStockItemPrice@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A0140, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 58.
// Identity: CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing&, long) const -- proven; evidence: FableWin.pdb name set holds
//   ?EntitySetStockItemPrice@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z (the catalog label agrees); it calls CTCStockItem::SetPrice
//   0x008ABBF0 (landed CTCStockItem::SetPrice) on the interface-type-0x54 component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCStockItem::SetPrice(long) 0x008ABBF0.
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
class CTCStockItem {
public:
    void SetPrice(long price);                              // 0x008ABBF0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing& thing, long price) const
{
    if (thing.IsAlive()) {
        CThing* pThing = thing.GetPThing();
        CTCStockItem* component;
        if (pThing->GetTC(TCI_STOCK_ITEM, &component))
            component->SetPrice(price);
    }
}