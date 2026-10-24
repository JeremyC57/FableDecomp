// std::__partial_sort(ENavigatorType* first, ENavigatorType* middle, ENavigatorType* last, ENavigatorType*, less<ENavigatorType> comp) -- STLport partial_sort core: heap the first part, push every smaller later element through it, then sort the heap
// Leaf: ??$__partial_sort@PAW4ENavigatorType@@
//
// Retail 0x00672330, 89 bytes (+ int3 padding to 96), `ret 0Ch` (/Gr: first in ecx, middle in edx; last, the _Tp*
//   tag and the empty 4-byte comparator on the stack).
// Callees: 0x00672410 make_heap(first, middle, comp) (out of line here, `ret 4`), 0x00672390 __adjust_heap(first, 0,
//   middle - first, value, comp), 0x00672460 sort_heap(first, middle, comp).
// Caller: 0x00672240 __introsort_loop (depth limit hit), part of sort() over CThingCreatureDef's
//   vector<ENavigatorType> (CThingCreatureDef::Transfer 0x00671600 sorts then uniques it).
// Shape: STLport _algo.c -- `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, _Distance*); sort_heap(first, middle, comp);` with __pop_heap
//   (`*result = *first; __adjust_heap(first, 0, last - first, value, comp)`) inline; less<> on the enum is the signed
//   `jge`, the loop test the unsigned pointer compare.  The retail TU is /Gr, written as explicit __fastcall.
// Label: catalog "Vector_PartialSort_Heap" (descriptive); STLport spelling __partial_sort.
enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& x, const _Tp& y) const { return x < y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp);                                 // 0x00672390
template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp);   // 0x00672410
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp);   // 0x00672460

template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result,
                       _Tp value, _Compare comp, _Distance*)
{
    *result = *first;
    __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator first, _RandomAccessIterator middle, _RandomAccessIterator last,
                               _Tp*, _Compare comp)
{
    make_heap(first, middle, comp);
    for (_RandomAccessIterator i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first));
    sort_heap(first, middle, comp);
}
}

template void __fastcall std::__partial_sort<ENavigatorType*, ENavigatorType, std::less<ENavigatorType> >(
    ENavigatorType*, ENavigatorType*, ENavigatorType*, ENavigatorType*, std::less<ENavigatorType>);