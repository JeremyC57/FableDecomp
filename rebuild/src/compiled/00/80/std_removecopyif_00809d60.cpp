// std::remove_copy_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, NPlayerTargeting::CTargetPoint* result, CFilter_Bind2<CThingFilter_IsInFOV,CTargetPointFilter_IsStealableStockItem> pred) -- copy the target points that are not stealable shop stock inside the field of view to result; returns the new end
// Flags: /Gr
// Leaf: remove_copy_if
// Retail: Fable.exe @ 0x00809D60, 213 bytes of code (/O2). Callees: CThingFilter_IsInFOV::operator()
// (const CTargetPoint&) 0x006A7380 (a 16-byte forwarder: `&tp.TargetingPosition` -> 0x006A7270; catalog label
// `_Cons_val<...EHeroMorphType...>` is a propagated name), CBaseIntelligentPointer::GetPItem 0x00A01B50,
// SetPItem 0x00A01B90, the const CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x004365B0,
// CTCStockItem::GetShopTC 0x007F4FC0.
// Shape: STLport `for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; } return
// result;` over the 28-byte NPlayerTargeting::CTargetPoint (as in 0x00807B70). The 8-byte CFilter_Bind2
// (Ego_r.pdb: F1 CThingFilter_IsInFOV* +0, F2 CTargetPointFilter_IsStealableStockItem* +4) is
// `(*F1)(t) && (*F2)(t)`; the empty stock-item filter is inline: the thing's TC_STOCK_ITEM (0x54) component
// bit (retail CThing UsingComponents bitset<274> at +0x20, Ego_r +0x20: word 2 bit 20), then the inline
// component lookup (CVectorMap at retail +0x44: LowerBound(&key), end read after it, `it == end || type < it->first`
// -> end, ->second; the landed CThingDeadCreature HasTC/GetTC forms),
// then GetShopTC() != 0 && CTCStockItem::Stealable (+0xD). /Gr: first=ecx, last=edx (spilled), result +
// the predicate's two pointers on the stack (`ret 0Ch`).
// Label correction: catalog `_Remove_if<CTargetPoint*,CFilter_Bind2<...>>` is the remove_copy_if half (the
// caller inlines find_if and calls this from the element after the hit).
namespace N00809d60 {
struct CBaseObject;
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
    void SetPItem(CThing* item);                          // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }      // user copy ctor, implicit operator=
};
namespace NPlayerTargeting {
struct CTargetPoint {                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;
struct CThingFilter_IsInFOV {
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& tp) const;        // 0x006A7380
};
struct CTargetPointFilter_IsStealableStockItem {          // _anon_58ED6966
    __forceinline bool operator()(const CTargetPoint& tp) const
    {
        const CThing* thing = tp.PThing.GetPItem();
        if (!thing->HasTC(TC_STOCK_ITEM))
            return false;
        const CTCStockItem* item = (const CTCStockItem*)thing->GetTC(TC_STOCK_ITEM);
        return item->GetShopTC() != 0 && item->Stealable;
    }
};
template <class TF1, class TF2> struct CFilter_Bind2 {
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
    template <class T> __forceinline bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t); }
};
typedef CFilter_Bind2<CThingFilter_IsInFOV, CTargetPointFilter_IsStealableStockItem> Pred;
template <class _InputIter, class _OutputIter, class _Predicate>
_OutputIter __fastcall remove_copy_if(_InputIter __first, _InputIter __last, _OutputIter __result, _Predicate __pred)
{
    for ( ; __first != __last; ++__first)
        if (!__pred(*__first)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}
template CTargetPoint* __fastcall remove_copy_if<CTargetPoint*, CTargetPoint*, Pred>(CTargetPoint*, CTargetPoint*, CTargetPoint*, Pred);
}