// std::__adjust_heap(long* first, int holeIndex, int len, long value, less<long> comp) -- STLport heap sift-down over 4-byte signed integers: move the larger child up until the hole reaches the bottom, then push value back up from there
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00776E50, 77 bytes, /Gr `__fastcall` `ret 0Ch` (first in ecx, holeIndex in
//   edx; len, the value and the empty comparator on the stack).
// Callee: __push_heap<long*,int,long,less<long> > @ 0x00776750 (landed; first ecx, hole edx, top/value/comp stacked).
// Callers: __partial_sort<long*> 0x007781B0 (this chunk) and the heap helpers calling from 0x007772DB / 0x0077738A /
//   0x007779DA -- the sort<long*, less<long> > instantiation of the landed sort_heap 0x00777EE0 (SIGNED compares: `jge`).
// Label correction: catalogued as `_Adjust_heap<CThing**,int,CThing*,CAdaptThingCompareToSTLComparitor<..> >`
//   (propagated); the element is a signed 4-byte integer and the body is STLport `__adjust_heap`.
// Shape: STLport `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) { if (comp(first
//   [secondChild], first[secondChild - 1])) secondChild--; first[holeIndex] = first[secondChild]; holeIndex =
//   secondChild; secondChild = 2 * (secondChild + 1); } if (secondChild == len) { first[holeIndex] = first[secondChild
//   - 1]; holeIndex = secondChild - 1; } __push_heap(first, holeIndex, topIndex, value, comp);`.
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);                                                  // 0x00776750

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap<long*, int, long, less<long> >(long*, int, int, long, less<long>);

}