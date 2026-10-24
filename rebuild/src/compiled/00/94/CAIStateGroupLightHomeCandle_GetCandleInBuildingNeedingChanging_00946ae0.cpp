// CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging(CThingBuilding& building) const -- the first candle in the building's owned-object list whose light needs changing (find_if with CIsCandleNeedingChanging), or 0 when the building owns nothing (no thing-owner component) or no candle qualifies
// Leaf: ?GetCandleInBuildingNeedingChanging@CAIStateGroup_LightHomeCandle
// Retail: Fable.exe (TLC, VC7.1) @ 0x00946AE0, 101 bytes (+ int3 padding to 112), `ret 4`.
// Callees: CVectorMap LowerBound @ 0x0040F020 (the inlined GetInterface of type 0x21 = CTCThingOwner: bitset word +0x24
//   bit 1), _STL::find_if<list const iterator, CIsCandleNeedingChanging> @ 0x00946D80 (/Gr __fastcall: the iterator
//   result by hidden pointer in ecx, first / last / the predicate on the stack), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50. The predicate's operator() is 0x00946C60 (called inside find_if).
// Shape: `if (GetInterface(&owner, 0x21)) { it = find_if(begin, end, CIsCandleNeedingChanging(this)); if (it != end)
//   return it->GetPItem(); } return 0;` -- end() is re-read after the call (`mov ecx,[esi+0Ch]`), the found node's
//   CIntelligentPointer sits at node+8. LowerBound and find_if are same-TU stand-ins under auto_inline(off): with them
//   VC7.1 sees the key and the result do not escape and gives both one frame word, as retail does (declaration-only,
//   the iterator gets its own slot).
// Layout: Ego_r.pdb CTCThingOwner::ObjectList at +0x0c (STLport list: the header node pointer; node = next, prev,
//   CIntelligentPointer<CThing> at +8); CIsCandleNeedingChanging = HomeCandle (4 bytes); CThing presence bitset +0x20,
//   interface map +0x44. The engine headers flatten these classes, so the model is local.
// Label: catalog "CAIStateGroup_LightHomeCandle_FindAndValidateTask" -> Ego_r.pdb
//   ?GetCandleInBuildingNeedingChanging@CAIStateGroup_LightHomeCandle@@ABEPAVCThing@@AAVCThingBuilding@@@Z.
// Build: the lane defaults (/O2 /Oy /W3), no pragma.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

// Same-TU stand-in for CVectorMap::LowerBound (its retail body is 0x0040F020, outside these bytes): a binary search
// that only reads the key, so the key's frame word can be reused for find_if's result.
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 presence bitset of the interface types
    CInterfaceMap m_interfaces;                             // +0x44 sorted {type, component} vector map
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingBuilding : public CThing { };

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                        // +0x04
    CThing* GetPItem() const;                               // 0x00A01B50
};
template <class T> struct CIntelligentPointer : public CBaseIntelligentPointer { };

namespace _STL {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
inline bool operator==(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _List_iterator_base& __x, const _List_iterator_base& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    _List_node_base* _M_node;                               // the header node
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred);   // 0x00946D80
}

struct CTCThingOwner {                                      // interface 0x21
    virtual void v00();
    unsigned char _pad04[0x8];
    _STL::list<CIntelligentPointer<CThing> > ObjectList;    // +0x0c
};

class CAIStateGroup_LightHomeCandle;
struct CIsCandleNeedingChanging {
    CAIStateGroup_LightHomeCandle* HomeCandle;              // +0x00 (Ego_r.pdb: a non-const pointer)
    CIsCandleNeedingChanging(const CAIStateGroup_LightHomeCandle* homeCandle)
        : HomeCandle(const_cast<CAIStateGroup_LightHomeCandle*>(homeCandle)) { }
    bool operator()(const CIntelligentPointer<CThing>& candle) const;   // 0x00946C60
};

// Same-TU stand-in for find_if (its retail body is 0x00946D80): STLport's loop, compiled out of line here.
#pragma auto_inline(off)
namespace _STL {
template <class _InputIter, class _Predicate>
_InputIter __fastcall find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}
#pragma auto_inline(on)

class CAIStateGroup_LightHomeCandle {
public:
    virtual void v00();
private:
    CThing* GetCandleInBuildingNeedingChanging(CThingBuilding& building) const;
};

CThing* CAIStateGroup_LightHomeCandle::GetCandleInBuildingNeedingChanging(CThingBuilding& building) const
{
    const CTCThingOwner* owner;
    if (building.GetInterface(&owner, 0x21)) {
        _STL::list<CIntelligentPointer<CThing> >::const_iterator it =
            _STL::find_if(owner->ObjectList.begin(), owner->ObjectList.end(), CIsCandleNeedingChanging(this));
        if (it != owner->ObjectList.end())
            return it->GetPItem();
    }
    return 0;
}