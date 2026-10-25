// std::__partial_sort(SPair* first, SPair* middle, SPair* last, SPair*, SLess comp) -- STLport partial_sort over 8-byte pair<long, unsigned long> records: heapify [first, middle), pop every smaller record of [middle, last) into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7760, 119 bytes, `ret 0Ch`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, middle in edx, then last, the value-type pointer tag and the 4-byte empty comparator slot on the stack.
// Callees: __make_heap 0x00AE62F0 (make_heap's out-of-line worker: comp, (T*)0, (int*)0 on the stack), __adjust_heap
//   0x00AE5FB0 (landed; reached through the inline __pop_heap), sort_heap 0x00AE71A0 (landed).
// Element: pair<long, unsigned long> -- STLport pair operator< (signed `jg/jl` on first, unsigned `jae` on second).
// Label: catalog "Std_PartialSort_Pair64BitMax" is descriptive.
// Shape: STLport 4.6 _algo.c verbatim (`make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, (int*)0); sort_heap(first, middle, comp);`) with __pop_heap inline
//   (`*result = *first; __adjust_heap(first, 0, middle - first, value, comp);`) -- the by-value copy of *i is read
//   before *first overwrites it, then rebuilt in the argument area by the pair's user copy constructor.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00AE62F0
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value, _Compare __comp);   // 0x00AE5FB0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00AE71A0

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __value, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __value, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}
}

typedef std::pair<long, unsigned long> SPair;
typedef std::less<SPair> SLess;

template void __fastcall std::__partial_sort<SPair*, SPair, SLess>(SPair*, SPair*, SPair*, SPair*, SLess);