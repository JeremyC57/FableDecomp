// std::__introsort_loop(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntry*, int depth_limit, CSurfaceEntryLess comp) -- introsort of the (key, CCompositeSurfaceInfo) entries down to 16-element runs: median-of-three partition, recurse on the right part, heap-sort fallback when the depth budget runs out
// Leaf: ??$__introsort_loop@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEF1D0, 217 bytes, `ret 0Ch` (value-type pointer, depth limit, comparator).
// Callees: __unguarded_partition @ 0x00BEDFE0, __partial_sort @ 0x00BEED30 (both this chunk), itself.
// Label: the manifest's "std_make_heap_build" is propagated; caller: sort @ 0x00BEF5C0 (landed) over the
// CVectorMap<long, CCompositeSurfaceInfo> entries.
// Element: see 0x00BED790. Shape: STLport 4.6 __introsort_loop (__stl_threshold 16: the byte distance is masked and
// compared with 0x100); __median and partial_sort inline; the depth limit is decremented in its argument slot.
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
template <class T, class Compare>
inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
{
    if (comp(a, b))
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp);   // 0x00BEDFE0

template <class RandomAccessIter, class T, class Compare>
void __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp);   // 0x00BEED30

template <class RandomAccessIter, class Compare>
inline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, (CSurfaceEntry*)0, comp);
}

#define __stl_threshold 16

template <class RandomAccessIter, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut =
            __unguarded_partition(first, last,
                                  T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                  comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}
}   // namespace std

template void std::__introsort_loop(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, int, CSurfaceEntryLess);