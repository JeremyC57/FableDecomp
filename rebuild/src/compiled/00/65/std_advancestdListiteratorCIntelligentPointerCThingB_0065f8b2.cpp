// std::advance<_List_iterator<CIntelligentPointer<CThingBuilding> >,int>(_List_iterator& i, int n) -- STLport advance:
// dispatch to __advance with the iterator's (bidirectional) category tag
// Leaf: ??$advance@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F8B2, 24 bytes, `__cdecl` (caller-cleaned): `lea eax,[ebp-1]` is the empty
//   bidirectional_iterator_tag temporary, then __advance(i, n, tag) (0x0065E8B8, landed) and `add esp,0Ch`.
// Identity (FINDINGS chunk 383; was filed as CEngineSceneGrid::GetCellBoundingBox, a free __cdecl char(ulong,ulong)
//   stand-in): its only caller CTCVillage::UpdateCurfew calls it at 0x0065AD99 with the address of a local iterator loaded
//   from `[[this+0x28]]` -- the first node of CTCVillage::VillageBuildings (struct_layouts_egor CTCVillage +0x28,
//   list<CIntelligentPointer<CThingBuilding>>) -- and the distance [this+0x8C]; its only callee is the landed
//   std::__advance<_List_iterator<CIntelligentPointer<CThingBuilding> >,int> 0x0065E8B8 (bidirectional, node next +0 /
//   prev +4). The iterator model is the landed __advance's (one template argument), so both TUs bind one symbol.
// Shape: STLport 4.x `advance(i, n) { __advance(i, n, _STLP_ITERATOR_CATEGORY(i, _InputIterator)); }` kept out of line
//   in a size TU (frame pointer, one-byte tag slot in the `push ecx` local).
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
struct bidirectional_iterator_tag { bidirectional_iterator_tag() { } };   // empty tag (repo idiom: the user-declared
                                                                       // constructor keeps VC7.1 from zero-filling the temporary)

struct _List_node_base {
    _List_node_base* _M_next;                     // +0x00
    _List_node_base* _M_prev;                     // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                  // +0x08
};
template <class _Tp> struct _List_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    _List_node_base* _M_node;
};

template <class _BidirectionalIterator, class _Distance>
void __advance(_BidirectionalIterator& __i, _Distance __n, const bidirectional_iterator_tag&);   // 0x0065E8B8

template <class _InputIterator, class _Distance>
void advance(_InputIterator& __i, _Distance __n)
{
    __advance(__i, __n, typename _InputIterator::iterator_category());
}

template void advance(_List_iterator<CIntelligentPointer<CThingBuilding> >&, int);
}
