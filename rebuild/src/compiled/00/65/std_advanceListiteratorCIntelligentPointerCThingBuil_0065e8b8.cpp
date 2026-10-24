// std::__advance<_List_iterator<CIntelligentPointer<CThingBuilding> >,int>(_List_iterator& i, int n, const bidirectional_iterator_tag&) -- step a list iterator n nodes forward (n > 0) or -n nodes back
// Leaf: ??$__advance@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065E8B8, 41 bytes, `__cdecl` (iterator reference, distance, the tag reference
// unused; caller-cleaned). Only caller: std::advance(iterator&, int) @ 0x0065F8B2 (builds the empty
// bidirectional_iterator_tag at [ebp-1] and passes three arguments, `add esp,0Ch`), itself called once from
// CTCVillage::UpdateCurfew at 0x0065AD99 on `VillageBuildings.begin()` (then GetPItem 0x00A01B50 on node+8), so the
// iterator is list<CIntelligentPointer<CThingBuilding>>::iterator.
// Label correction: the catalog "BinaryTree_Navigate" is descriptive; this is STLport `__advance` for bidirectional
// iterators. (The landed caller 0x0065F8B2 is filed as CEngineSceneGrid::GetCellBoundingBox -- it is std::advance.)
// Shape: STLport verbatim: `if (n > 0) while (n--) ++i; else while (n++) --i;` -- VC7.1 turns both loops into
// count-down `dec eax ; jne` loops (the second on -n), node next +0, prev +4.
// Opt: size TU (`optimize("s", on)`).
#pragma optimize("s", on)

class CThing;
class CThingBuilding;
class CBaseIntelligentPointer {                   // 8 bytes: vptr + item
public:
    virtual void v00();
    CThing* m_pItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

namespace std {
struct bidirectional_iterator_tag { };

struct _List_node_base {
    _List_node_base* _M_next;                     // +0x00
    _List_node_base* _M_prev;                     // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                  // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
    _List_iterator& operator--() { this->_M_decr(); return *this; }
};

template <class _BidirectionalIterator, class _Distance>
void __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&)
{
    if (__n > 0)
        while (__n--) ++__i;
    else
        while (__n++) --__i;
}

template void __advance(_List_iterator<CIntelligentPointer<CThingBuilding> >&, int, const bidirectional_iterator_tag&);
}