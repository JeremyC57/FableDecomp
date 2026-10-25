// std::__partial_sort(pair<unsigned long,unsigned long>* first, pair<unsigned long,unsigned long>* middle, pair<unsigned long,unsigned long>* last, pair<unsigned long,unsigned long>*, less<pair<unsigned long,unsigned long> > comp) -- heap-select the smallest (middle - first) pairs into [first, middle) and heap-sort them
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6BBB0, 119 bytes, `ret 0Ch` (link-time __fastcall: first in ecx,
//   middle in edx, last, the value-type tag pointer and the empty comparator on the stack).
// Callees: 0x00A6B780 std::__make_heap(first, last, comp, T*, ptrdiff_t*) (the two null tag pointers are the
//   `push 0 ; push 0`), 0x00A6B4F0 std::__adjust_heap(first, hole, len, val, comp) (catalogued _Adjust_heap of
//   pair<unsigned long,unsigned long>), 0x00A6B990 std::sort_heap(first, last, comp).
// Callers: 0x00A6BDFC, 0x00A6BFAC (the same TU's partial_sort / introsort depth-limit fallback).
// Identity: the manifest's "HeapSort_PartialSort_u64PairLarge" is descriptive. Element = std::pair<unsigned long,
//   unsigned long> under the default less<pair> (unsigned `ja/jb` on first, `jae` on second).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (ptrdiff_t*)0); sort_heap(first, middle, comp);` with make_heap's
//   front end and __pop_heap (`*result = *first; __adjust_heap(first, 0, last - first, val, comp)`) inlined; the
//   value copy T(*i) is taken before the `*i = *first` store (its words in ecx/edx) and rebuilt in the argument area.
namespace std {

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> class less {
public:
    less() { }
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00A6B780

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A6B4F0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00A6B990

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

typedef pair<unsigned long, unsigned long> TPair;
template void __fastcall __partial_sort<TPair*, TPair, less<TPair> >(TPair*, TPair*, TPair*, TPair*, less<TPair>);

}