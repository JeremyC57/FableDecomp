// std::__make_heap(long* first, long* last, less<long> comp, long*, int*) -- STLport heap construction over 4-byte signed integers: sift down every parent from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007772F0, 124 bytes, /Gr `__fastcall` `ret 0Ch` (first in ecx -> esi, last in
//   edx; the empty comparator, the value-type and distance-type tag pointers on the stack).
// Callee: __push_heap<long*,int,long,less<long> > @ 0x00776750 (landed).
// Callers: __partial_sort<long*> 0x007781B0 (this chunk) and the make_heap front end calling from 0x007779A9.
// Label correction: catalogued as `MakeHeap`; the body is STLport `__make_heap` for the sort<long*, less<long> > family
//   (SIGNED compares) with `__adjust_heap` expanded inline (its out-of-line copy is 0x00776E50).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the value is
//   spilled to the `push ecx` frame word for the __push_heap call, and `2 * parent + 2` is strength-reduced (`sub ebp,2`).
//   __adjust_heap must be `__forceinline`: plain `inline` leaves it a __cdecl call (81 bytes).
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);                                                  // 0x00776750

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
__forceinline void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
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

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __fastcall __make_heap<long*, less<long>, long, int>(long*, long*, less<long>, long*, int*);

}