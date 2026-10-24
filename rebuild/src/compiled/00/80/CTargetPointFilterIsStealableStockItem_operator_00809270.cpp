// CTargetPointFilter_IsStealableStockItem::operator()(const NPlayerTargeting::CTargetPoint& point) -- true when the target point's thing is shop stock (stock-item component) that belongs to a shop and is flagged stealable
// Leaf: ??RCTargetPointFilter_IsStealableStockItem@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00809270, 89 bytes (+ int3 padding), __fastcall (`this` unused: the
//   filter is empty), `ret 4`.
// Name: Ego_r.pdb `anonymous namespace'::CTargetPointFilter_IsStealableStockItem` (NPlayerTargeting TU, _anon_58ED6966);
//   the same body is inlined into the landed remove_copy_if 0x00809D60. The catalog's "CTCStockItem_IsSpecialItem" is
//   descriptive.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//   0x004365B0, CTCStockItem::GetShopTC 0x007F4FC0.
// Layout: retail CThing UsingComponents bitset +0x20 (TC_STOCK_ITEM 0x54 = word 2 bit 20), components CVectorMap +0x44
//   (entries {type, component}); CTCStockItem Stealable +0x0D.
// Shape: the landed 0x00809D60 HasTC / GetTC forms -- HasTC as `if (bit) return true; return false;` (the masked
//   `test dword ptr`), GetTC with the key homed in the dead `point` argument slot, `it == end || type < it->first ? end
//   : it` with end read after the lookup; then `GetShopTC() != 0 && Stealable` as `if (..) return true;` inside the
//   HasTC block and one shared `return false` (the `mov al,1` / shared `xor al,al` tail; `return a && b` gives
//   `mov eax,1` and an early `return false` duplicates the epilogue).
struct CTCBase;
struct CTCShop;
enum ETCInterfaceType { TC_STOCK_ITEM = 0x54, TC_FORCE_DWORD = 0x7fffffff };
template <class K, class V> struct pair { K first; V second; };
struct CTCMap {                                           // CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...>>
    typedef pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                 // +0x0 (STLport vector)
    value_type* _M_finish;                                // +0x4
    value_type* _M_end_of_storage;                        // +0x8
    value_type* LowerBound(const ETCInterfaceType* key) const;   // 0x004365B0 (const copy)
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                     // +0x20 bitset<274>
    CTCMap Components;                                    // +0x44
    __forceinline bool HasTC(ETCInterfaceType type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const CTCMap* map = &Components;
        CTCMap::value_type* it = map->LowerBound(&key);
        CTCMap::value_type* end = map->_M_finish;
        return (it == end || type < it->first ? end : it)->second;
    }
};
struct CTCStockItem {
    unsigned char _pad00[0xc];
    bool ForSale;                                         // +0x0C
    bool Stealable;                                       // +0x0D
    bool Initialised;                                     // +0x0E
    long Price;                                           // +0x10
    CTCShop* GetShopTC() const;                           // 0x007F4FC0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                      // +0x04
    CThing* GetPItem() const;                             // 0x00A01B50
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { };
struct C3DVector { float X, Y, Z; };
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

namespace {
struct CTargetPointFilter_IsStealableStockItem {
    bool operator()(const CTargetPoint& point) const;
};
}

bool CTargetPointFilter_IsStealableStockItem::operator()(const CTargetPoint& point) const
{
    const CThing* thing = point.PThing.GetPItem();
    if (thing->HasTC(TC_STOCK_ITEM)) {
        const CTCStockItem* item = (const CTCStockItem*)thing->GetTC(TC_STOCK_ITEM);
        if (item->GetShopTC() != 0 && item->Stealable)
            return true;
    }
    return false;
}