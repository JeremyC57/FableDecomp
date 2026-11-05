// CTCShop::~CTCShop() -- compiler-generated shop thing-component destructor: release the stock map, the goods places, the shoppers' intelligent pointers and the shopkeeper pointer, then the CTCBase base
// Leaf: ??1CTCShop@@UAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007783F0, 145 bytes, `jmp` to the base destructor.
// Identity (proven): the sole caller is the scalar deleting destructor 0x007783D0, slot 0 of vtable 0x0126A94C (RTTI
//   CTCShop : CTCBase); the tail jump is ~CTCBase 0x00686830 (direct base, RTTI). Ego_r.pdb ??1CTCShop@@UAE@XZ (0x00784050)
//   / ??_G (0x007840D0), FableWin.pdb the same pair (0x02618F90 / 0x02618F30). The catalog label
//   CPatchTesselationEdgeStrip::~CPatchTesselationEdgeStrip is a propagation.
// Callees: std::_Rb_tree<long, pair<const long, CShopItem>, ...>::_M_erase 0x00776AA0 (map<long, CShopItem>), free
//   0x00BFEA14, the element's virtual destructor (slot 0, flag 0), CBaseIntelligentPointer::~CBaseIntelligentPointer
//   0x00A01C10, CTCBase::~CTCBase 0x00686830.
// Layout (Ego_r.pdb members at retail STLport sizes): StockState +0x0c, PShopKeeper +0x10, Shoppers +0x18,
//   ShopKeeperAvailable +0x24, GoodsPlaces +0x28, Stock +0x34, SpecialItem +0x40, DaysSinceSpecialItemPlaced +0x44,
//   DayLastChecked +0x48, GoldDonated +0x4c, StallLike +0x50, Alarm +0x54 (0x58 bytes).
// Shape: no vtable store (compiler-generated destructor); reverse member order: ~map (clear + header free), ~vector of
//   the trivially destroyed GoodsPlace (free only), ~vector of CIntelligentPointer (STLport by-value _Destroy(first, last):
//   end cached in edi, explicit virtual destructor per 8-byte element, then free), the shopkeeper pointer's inline
//   implicit dtor calling the base pointer dtor, then the out-of-line CTCBase destructor as a tail jump.
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14 (allocator::deallocate)
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                                // +0x00
    _Base_ptr   _M_parent;                               // +0x04
    _Base_ptr   _M_left;                                 // +0x08
    _Base_ptr   _M_right;                                // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
protected:
    typedef _Rb_tree_node<_Value> _Node;
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
protected:
    size_type _M_node_count;                             // +0x04
    _Compare  _M_key_compare;                            // +0x08
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
private:
    void _M_erase(_Link_type __x);                       // out of line, one instantiation per tree type
public:
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;                                      // +0x00 (12 bytes)
};
} // namespace std
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp> inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, &*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;                                       // +0x00
    _Tp* _M_finish;                                      // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector (12 bytes)
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CTCBase : public CBaseClassNonCopyable {          // RTTI CTCBase : CBaseClassNonCopyable; 0xC bytes (engine/CTCBase.h offsets)
public:
    virtual ~CTCBase();                                 // 0x00686830
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/C3DVector.h"
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CThingCreatureBase;
class CShopItem {                                       // Ego_r.pdb, 0x1C bytes
public:
    float Quantity;                                     // +0x00
    float PriceMultiplier;                              // +0x04
    float MaxStock;                                     // +0x08
    long  RestockPeriod;                                // +0x0c
    float RestockPeriodRandomness;                      // +0x10
    float SalesPerDay;                                  // +0x14
    float SalesPerDayRandomness;                        // +0x18
};
enum ESTOCK_STATE { };

class CTCShop : public CTCBase {
public:
    struct GoodsPlace {
        C3DVector       pos;                            // +0x00
        CRightHandedSet rhset;                          // +0x0c
    };
    CTCShop(CThing* thing);                             // 0x007744A0
    ESTOCK_STATE                                          StockState;            // +0x0c
    CIntelligentPointer<CThingCreatureBase>               PShopKeeper;           // +0x10
    std::vector<CIntelligentPointer<CThingCreatureBase> > Shoppers;              // +0x18
    bool                                                  ShopKeeperAvailable;   // +0x24
    std::vector<GoodsPlace>                               GoodsPlaces;           // +0x28
    std::map<long, CShopItem>                             Stock;                 // +0x34
    long                                                  SpecialItem;           // +0x40
    long                                                  DaysSinceSpecialItemPlaced;   // +0x44
    long                                                  DayLastChecked;        // +0x48
    long                                                  GoldDonated;           // +0x4c
    bool                                                  StallLike;             // +0x50
    unsigned long                                         Alarm;                 // +0x54
};

// TU harness (not a retail function): destroying a local by name makes VC7.1 emit the implicit ??1 COMDAT.
void TUHarness_CTCShop(CThing* thing)
{
    CTCShop shop(thing);
}