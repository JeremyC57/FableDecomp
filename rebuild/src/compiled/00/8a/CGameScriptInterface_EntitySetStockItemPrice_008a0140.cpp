// CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing& thing, long price) const -- when the script thing is valid and its thing has a CTCStockItem component (interface type 0x54), call CTCStockItem::SetPrice(price)
// Leaf: ?EntitySetStockItemPrice@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A0140, 91 bytes (+ int3 padding), thiscall, `ret 8`; CGameScriptInterface vtable 0x012615B8 slot 58.
// Identity: CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing&, long) const -- proven; evidence: FableWin.pdb name set holds
//   ?EntitySetStockItemPrice@CGameScriptInterface@@UBEXABVCScriptThing@@J@Z (the catalog label agrees); it calls CTCStockItem::SetPrice
//   0x008ABBF0 (landed CTCStockItem::SetPrice) on the interface-type-0x54 component. Family G045 (8 script setters).
// Callees: the script thing's virtual slot 75 (+0x12C, IsValid) and slot 11 (+0x2C, GetThing), CInterfaceMap::LowerBound
//   0x0040F020 (the thing's {type, component} map at +0x44), CTCStockItem::SetPrice(long) 0x008ABBF0.
// Shape: `this` unused; the GetInterface template is `__forceinline` and the lookup key lives in the dead `thing` argument
//   slot; the component is not null-tested; the value is forwarded as a dword.
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                       // a thing's sorted {type, component} vector map
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                          // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                              // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
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
        if (pThing->GetInterface(&component, 0x54))
            component->SetPrice(price);
    }
}