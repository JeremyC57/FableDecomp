// std::_Push_heap(std::pair<long,CDefPointer<const CContextSensitiveItemDef> >* first, int hole, int top, std::pair<long,CDefPointer<const CContextSensitiveItemDef> > val, CContextSensitivePriorityCompare pred) -- percolate a context-sensitive item def up a heap ordered by its priority, then destroy the by-value pair
// Flags: /Gr
// Leaf: _Push_heap
// Retail: Fable.exe @ 0x00593965, 98 bytes, size-optimised region with an ebp frame (unaligned;
// `optimize("y", off)` + `("s", on)`). Callees: CContextSensitivePriorityCompare::operator()
// 0x005928A8, pair<long,CDefPointer<..> >::operator= 0x0059308A (out of line in this TU),
// pair<long,CDefPointer<..> >::~pair 0x005930F3.
// Shape: STLport `__push_heap` (catalog `_Push_heap`) (`hole > top` test, unrotated: `jmp` to the test) over an
// 8-byte pair (long key +0, CDefPointer +4). The predicate is a real (out-of-line) member functor
// called with `this` = the by-value comparator slot ([ebp+14h]) and (first[idx], val) pushed right
// to left; the pair's assignment and destructor are separate functions here.
// /Gr: first=ecx, hole=edx; `ret 10h` = top + 8-byte pair + the 4-byte comparator slot.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace N00593965 {
class CContextSensitiveItemDef;
template<class T> struct CDefPointer {
    T* Def;
};
template<class K, class V> struct pair {
    K first;
    V second;
    pair& operator=(const pair& other);                   // 0x0059308A
    ~pair();                                              // 0x005930F3
};
typedef pair<long, CDefPointer<const CContextSensitiveItemDef> > Elem;
struct CContextSensitivePriorityCompare {
    bool operator()(const Elem& a, const Elem& b) const;  // 0x005928A8
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
template void _Push_heap<Elem*, int, Elem, CContextSensitivePriorityCompare>(Elem*, int, int, Elem, CContextSensitivePriorityCompare);
}