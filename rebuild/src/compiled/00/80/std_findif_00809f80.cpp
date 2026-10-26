// std::__find_if(NPlayerTargeting::CTargetPoint* first, NPlayerTargeting::CTargetPoint* last, CFilter_Bind2 pred, const random_access_iterator_tag&) -- STLport random-access find_if: the first target point inside the field of view whose thing is stealable shop stock, or last
// Leaf: ??$__find_if@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00809F80, 382 bytes, /Gr `__fastcall` (first ecx, last edx -- spilled to a frame
//   word; the 8-byte CFilter_Bind2 by value and the tag reference on the stack, `ret 0Ch`).
// Identity: Ego_r.pdb `std::_Find_if<CTargetPoint*, CFilter_Bind2<CThingFilter_IsInFOV, `anonymous namespace'::
//   CTargetPointFilter_IsStealableStockItem> >` (donor 0x00803420) is the Dinkumware counterpart; retail is STLport's
//   `__find_if` (callers 0x0080477C, 0x0080A83F, 0x0080AF54). The catalog's "std_copy_if_IsSpecialItem" is wrong
//   (nothing is copied: the find_if half of the remove_if whose remove_copy_if half is the landed 0x00809D60).
// Callees: CThingFilter_IsInFOV::operator()(const CTargetPoint&) const 0x006A7380, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, the const CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0, CTCStockItem::GetShopTC
//   0x007F4FC0, CTargetPointFilter_IsStealableStockItem::operator() 0x00809270 (landed; out of line at six sites).
// Shape: STLport 4.x `__find_if` (trip count `(last - first) >> 2`, four tests per pass, `switch (last - first)` over
//   3/2/1); the bind is `(*F1)(t) && (*F2)(t)`; VC7.1's per-function inline budget expands the stock-item filter (the
//   landed 0x00809270 body, plain inline) at the first site only and calls it out of line at the other six.
class CTCBase;
class CTCShop;
enum ETCInterfaceType { TC_STOCK_ITEM = 0x54, TC_FORCE_DWORD = 0x7fffffff };
template <class K, class V> struct pair { K first; V second; };
struct CTCMap {                                           // CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...>>
    typedef pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                 // +0x0 (STLport vector)
    value_type* _M_finish;                                // +0x4
    value_type* _M_end_of_storage;                        // +0x8
    value_type* LowerBound(const ETCInterfaceType* key) const;   // 0x004365B0 (const copy)
};
class CThing { public:
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
class CTCStockItem { public:
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };
class C3DVector { public: float X, Y, Z; };
namespace NPlayerTargeting {
class CTargetPoint { public:                                     // 0x1c bytes
    CIntelligentPointer<CThing> PThing;                   // +0x00
    long Index;                                           // +0x08
    C3DVector TargetingPosition;                          // +0x0C
    long FrameRejected;                                   // +0x18
};
}
using NPlayerTargeting::CTargetPoint;

class CThingFilter_IsInFOV {                              // Ego_r.pdb, 0x24 bytes
public:
    unsigned char _pad00[0x24];
    bool operator()(const CTargetPoint& point) const;     // 0x006A7380
};

namespace {
class CTargetPointFilter_IsStealableStockItem {           // Ego_r.pdb, empty
public:
    bool operator()(const CTargetPoint& point) const      // the landed 0x00809270 body
    {
        const CThing* thing = point.PThing.GetPItem();
        if (thing->HasTC(TC_STOCK_ITEM)) {
            const CTCStockItem* item = (const CTCStockItem*)thing->GetTC(TC_STOCK_ITEM);
            if (item->GetShopTC() != 0 && item->Stealable)
                return true;
        }
        return false;
    }
};
}

template <class TF1, class TF2> class CFilter_Bind2 {
public:
    TF1* F1;                                              // +0x0
    TF2* F2;                                              // +0x4
    template <class T> bool operator()(const T& t) const { return (*F1)(t) && (*F2)(t); }
};
typedef CFilter_Bind2<CThingFilter_IsInFOV, CTargetPointFilter_IsStealableStockItem> Pred;

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}

template CTargetPoint* __fastcall __find_if(CTargetPoint*, CTargetPoint*, Pred, const random_access_iterator_tag&);
}