// std::list::remove(const value_type& val) -- (list<CIntelligentPointer<CThingBuilding > >) STLport: erase every node whose pointer equals val (here the null pointer CTCVillage::ClearOutNullMembers passes)
// Leaf: ?remove@?$list@V?$CIntelligentPointer@VCThingBuilding@@@@V?$allocator@V?$CIntelligentPointer@VCThingBuilding@@@@@std@@@std@@QAEXABV?$CIntelligentPointer@VCThingBuilding@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065DDDB, 66 bytes (unaligned, frame pointer: a size-optimised TU), thiscall `ret 4`.
// Identity (proven): std::list<CIntelligentPointer<CThingBuilding > >::remove; evidence: CTCVillage::ClearOutNullMembers 0x0065890B calls it at 0x00658926 on VillageBuildings +0x28 (Ego_r.pdb CTCVillage list<CIntelligentPointer<CThingBuilding> > member; the retail 4-byte lists run from +0x28 in PDB order, the 12-byte PlayersCrimes set before VillageMarkers +0x70) with a null CIntelligentPointer<CThingBuilding> temporary; its equality callee 0x0065D655 is that element's own operator== instantiation.
// Label: the manifest's "SetScriptBrushThingsToMove" is propagated (family H092: six list<CIntelligentPointer<T> >::remove COMDATs, one per T).
// Callees: operator==(const CIntelligentPointer<CThingBuilding >&, const CIntelligentPointer<CThingBuilding >&) 0x0065D655 (__fastcall, PDB
//   ??$?8...@@YI_N...), list::erase(iterator) 0x0065DDB0 (hidden return pointer, `ret 8`).
// Shape: STLport 4.6 list<T>::remove -- `if (__val == *__first) erase(__first);` with the successor taken first; the list is the 4-byte
//   STLport head-node pointer; the iterator's user copy constructor builds erase's by-value argument in place (`push ecx` then a
//   store through esp). Local classes -- Review at landing.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThingBuilding;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    void* PItemInfo;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
};
template <class T> bool __fastcall operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b);
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                                 // +0x00
    _List_node_base* _M_prev;                                 // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                              // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator() { }
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }   // STLport's user copy constructor
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;                                 // +0x00: the head (sentinel) node
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    iterator erase(iterator __position);                      // 0x0065DDB0
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first) erase(__first);
            __first = __next;
        }
    }
};
}
template void std::list<CIntelligentPointer<CThingBuilding > >::remove(const CIntelligentPointer<CThingBuilding >& __val);