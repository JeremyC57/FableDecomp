// _STL::__partial_sort(float* first, float* middle, float* last, float*, less<float> comp) -- heap-select the smallest floats of [first, last) into [first, middle) and sort them
// Leaf: ??$__partial_sort@PAMMU?$less@M@_STL@@@_STL@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5A8A0, 102 bytes (+10 int3), `ret 0Ch` (link-time __fastcall: first in
//   ecx, middle in edx, then last, the value-type tag pointer and the empty less<float> on the stack).
// Callees: __make_heap @ 0x00A5A660 (first, middle, comp, (float*)0, (int*)0), __adjust_heap @ 0x00A5A580 (the inlined
//   __pop_heap; catalogued as the propagated `_Adjust_heap<CNavigatorAStar::CAStarState**,...>`), sort_heap @ 0x00A5A7D0.
// Label: the manifest's `Quicksort_Float_Partial` is descriptive; the body is STLport 4.6's
//   `__partial_sort<float*, float, less<float> >`.
// Shape: `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap
//   and __pop_heap inline: the by-value T(*i) temporary is homed in the comparator's dead argument slot, the comparator's
//   word is kept in ebp for all three calls.
#include <stddef.h>

namespace _STL {

template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};

template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);                 // 0x00A5A660

template <class RandomAccessIter, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T val, Compare comp);           // 0x00A5A580

template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);                                 // 0x00A5A7D0

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (float*)0, (ptrdiff_t*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (ptrdiff_t*)0);
    sort_heap(first, middle, comp);
}

template void __partial_sort(float*, float*, float*, float*, less<float>);

}   // namespace _STL