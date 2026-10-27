// std::__find_if(_List_iterator<const CThingBuilding*> first, last, NCameraModes::CIsPosInBuilding pred, const input_iterator_tag&) -- first building in the list that contains the position
// Leaf: ??$__find_if@U?$_List_iterator@PBVCThingBuilding@@U?$_Nonconst_traits@PBVCThingBuilding@@@std@@@std@@VCIsPosInBuilding@NCameraModes@@@std@@YI?AU?$_List_iterator@PBVCThingBuilding@@U?$_Nonconst_traits@PBVCThingBuilding@@@std@@@0@U10@0VCIsPosInBuilding@NCameraModes@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe @ 0x00877FD0, 52 bytes, __fastcall (hidden iterator result in ecx, the empty tag by reference in edx,
// never read; first / last / the 4-byte predicate on the stack), `ret 0Ch`.
// Identity (proven): STLport 4.6 `while (first != last && !pred(*first)) ++first; return first;` over a list (node next +0,
// value +8) of CThingBuilding pointers, the predicate calling CThingBuilding::IsPointInside(*pos) (0x0082D000) with its one
// word. Its callers are the out-of-line std::find_if 0x00878010 and FindIf 0x00878040 (returns bool and stores the found
// building), plus an inlined find_if at 0x008776D8 after CTools::GetThingsOfType<CThingBuilding> (0x00878390). FableWin.pdb
// holds exactly one find_if / _Find_if / FindIf over list<const CThingBuilding*>, with the predicate
// NCameraModes::CIsPosInBuilding = { C3DVector* Pos } and ??RCIsPosInBuilding@NCameraModes@@QBE_NPBVCThingBuilding@@@Z.
// Label correction: catalog "_gapscan::sub_00877fd0".
// Callees: 0x0082D000 ?IsPointInside@CThingBuilding@@QBE_NABVC3DVector@@@Z (Ego_r.pdb: const).
// Shape: the predicate's position word stays in ebx across the loop; the empty-range test precedes it.
class C3DVector {
public:
    float X, Y, Z;
};

class CThingBuilding {
public:
    bool IsPointInside(const C3DVector& pos) const;         // 0x0082D000
};

namespace NCameraModes {
class CIsPosInBuilding {
public:
    bool operator()(const CThingBuilding* building) const { return building->IsPointInside(*Pos); }
    C3DVector* Pos;                                         // +0x00
};
}

namespace std {
struct input_iterator_tag { };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};

template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}

typedef _List_iterator<const CThingBuilding*, _Nonconst_traits<const CThingBuilding*> > _BuildingListIter;
template _BuildingListIter __fastcall __find_if(_BuildingListIter, _BuildingListIter, NCameraModes::CIsPosInBuilding,
                                                const input_iterator_tag&);
}