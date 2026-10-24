// std::__partial_sort(long* first, long* middle, long* last, long*, less<long> comp) -- STLport partial sort over 4-byte signed integers: heapify [first, middle), pop every smaller later element into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007781B0, 89 bytes (+ int3 padding), /Gr `__fastcall` `ret 0Ch` (first in ecx
//   -> esi, middle in edx -> ebx; last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap(first, middle, comp, (T*)0, (int*)0) @ 0x007772F0 (this chunk), __adjust_heap @ 0x00776E50 (this
//   chunk), sort_heap @ 0x00777EE0 (landed).
// Callers: __introsort_loop<long*> 0x00778490 (this chunk, depth limit exhausted) and 0x0077836C.
// Label correction: catalogued as `Sort_PartialHeap_ThingNearest`; the element is a signed 4-byte integer (the
//   sort<long*, less<long> > family) and the body is STLport `__partial_sort`.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap
//   forwarding to the out-of-line __make_heap and __pop_heap inline (`*result = *first` then the out-of-line
//   __adjust_heap with hole 0 and len = middle - first).
namespace std {

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*,
                            _Distance*);                                                       // 0x007772F0
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                               // 0x00776E50
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00777EE0

template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort<long*, long, less<long> >(long*, long*, long*, long*, less<long>);

}