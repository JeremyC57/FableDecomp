// std::__insertion_sort(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntryLess comp) -- insertion sort of the (key, CCompositeSurfaceInfo) entries: linear insert of each entry after the first
// Leaf: ??$__insertion_sort@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEECD0, 81 bytes, `ret 4` (the empty comparator).
// Callees: __linear_insert @ 0x00BEE8F0 (this chunk).
// Label: the manifest's "std_vector_destroy_with_callback" is propagated nonsense; caller: sort @ 0x00BEF5C0 (landed,
// its __final_insertion_sort inlined) over the CVectorMap<long, CCompositeSurfaceInfo> entries.
// Element: see 0x00BED790. Shape: STLport 4.6 __insertion_sort; the by-value argument T(*i) is copy-constructed in its
// outgoing slot (key, vptr 0x012A8154, counted-pointer copy).
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
template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp);   // 0x00BEE8F0

template <class RandomAccessIter, class Compare>
void __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last) return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, CSurfaceEntry(*i), comp);
}
}   // namespace std

template void std::__insertion_sort(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess);