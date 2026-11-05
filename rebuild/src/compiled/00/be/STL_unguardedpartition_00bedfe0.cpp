// std::__unguarded_partition(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntry pivot, CSurfaceEntryLess comp) -- Hoare partition of the (key, CCompositeSurfaceInfo) entries around pivot's key, returns the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEDFE0, 127 bytes, `ret 14h` (the 16-byte pivot + the empty comparator).
// Callees: iter_swap @ 0x00BED690 (out of line, __fastcall), operator delete @ 0x00BFE9BC and ~CBaseClass @ 0x0099A2E0
// (the by-value pivot's implicit destructor).
// Label: the manifest's "Std_HeapPartition" is descriptive; this is the STLport introsort partition (caller:
// __introsort_loop @ 0x00BEF1D0, this chunk) of the CVectorMap<long, CCompositeSurfaceInfo> entries.
// Element: see 0x00BED790. Shape: STLport 4.6 __unguarded_partition; the key compares are inline and signed.
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
template <class ForwardIter1, class ForwardIter2>
void iter_swap(ForwardIter1 a, ForwardIter2 b);   // 0x00BED690

template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            ++first;
        --last;
        while (comp(pivot, *last))
            --last;
        if (!(first < last))
            return first;
        iter_swap(first, last);
        ++first;
    }
}
}   // namespace std

template CSurfaceEntry* std::__unguarded_partition(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry, CSurfaceEntryLess);