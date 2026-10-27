// std::__push_heap(CThing** first, int holeIndex, int topIndex, CThing* val, CCompare_FurthestFromInitPos2D comp) -- STLport heap sift-up: while the hole is below the top and comp(parent, val) holds, move the parent down; then store val in the hole
// Leaf: ??$__push_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748880, 97 bytes, /Gr __fastcall (first ecx, holeIndex edx; topIndex, val and the
//   comparator on the stack), `ret 0Ch`.
// Identity (proven): only caller the landed std::__adjust_heap<CThing**, int, CThing*, CCompare_FurthestFromInitPos2D> 0x00748C90 (which declares this callee at 0x00748880); comparator 0x007483E0 (landed); 4-byte functor -> `ret 0Ch`.
// Shape (family K019, representative 0x00748880): STLport 4.6 _heap.c `parent = (hole - 1) / 2; while (hole > top &&
//   comp(*(first + parent), val)) { *(first + hole) = *(first + parent); hole = parent; parent = (hole - 1) / 2; }
//   *(first + hole) = val;` (SGI `hole > top`, signed divides by 2 through cdq/sub/sar).
class CThing;
// The landed 0x007483E0 predicate (an empty functor: one argument dword).
struct CCompare_FurthestFromInitPos2D {
    bool operator()(CThing* a, CThing* b) const;          // 0x007483E0
};
typedef CCompare_FurthestFromInitPos2D TComp;

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template void __fastcall __push_heap<CThing**, int, CThing*, TComp>(CThing**, int, int, CThing*, TComp);
} // namespace std