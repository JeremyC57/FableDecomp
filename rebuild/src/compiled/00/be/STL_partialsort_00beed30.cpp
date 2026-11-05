// std::__partial_sort(CSurfaceEntry* first, CSurfaceEntry* middle, CSurfaceEntry* last, CSurfaceEntry*, CSurfaceEntryLess comp) -- heap-select the smallest (key, CCompositeSurfaceInfo) entries into [first, middle) and sort them (introsort's depth-limit fallback)
// Leaf: ??$__partial_sort@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEED30, 111 bytes, `ret 0Ch` (last, value-type pointer, comparator).
// Callees: __make_heap @ 0x00BEE140 (this chunk), __pop_heap @ 0x00BEE070 (pending), sort_heap @ 0x00BEE990 (landed).
// Label: the manifest's "Std_Heap_InsertAndSort" is descriptive; caller: __introsort_loop @ 0x00BEF1D0 (this chunk).
// Element: see 0x00BED790. Shape: STLport 4.6 __partial_sort with make_heap inline (`push 0 ; push 0 ; push comp`);
// the by-value T(*i) of each __pop_heap is copy-constructed in its outgoing slot.
#include <new>
#include <stddef.h>

// CBaseClass: the polymorphic engine root (out-of-line virtual destructor 0x0099A2E0, vtable 0x01231710).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/CCountedPointer.h"


class CCompositeSurfaceCache;
// Ego_r.pdb CCompositeSurfaceInfo (vtable 0x012A8154, 12 bytes): implicit copy, assignment and (virtual) destructor.
class CCompositeSurfaceInfo : public CBaseClass {
public:
    CCountedPointer<CCompositeSurfaceCache> Cache;  // +0x04
};

namespace std {
struct __false_type { __false_type() { } };
struct __true_type { __true_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
}   // namespace std

typedef std::pair<long, CCompositeSurfaceInfo> CSurfaceEntry;   // 0x10 bytes

// The vector map's key order (an empty functor passed by value).
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

namespace std {
template <class RandomAccessIter, class Compare, class T, class Distance>
void __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);   // 0x00BEE140

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (CSurfaceEntry*)0, (ptrdiff_t*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*);   // 0x00BEE070

template <class RandomAccessIter, class Compare>
void sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);   // 0x00BEE990

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (ptrdiff_t*)0);
    sort_heap(first, middle, comp);
}
}   // namespace std

template void std::__partial_sort(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess);