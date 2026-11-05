// std::__unguarded_insertion_sort_aux(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntry*, CSurfaceEntryLess comp) -- unguarded linear insert of every (key, CCompositeSurfaceInfo) entry of [first, last)
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEDD20, 71 bytes, `ret 8` (value-type pointer + empty comparator).
// Callees: __unguarded_linear_insert @ 0x00BED790 (this chunk).
// Label: the manifest's "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated; this is the STLport sort
// instantiation of the CVectorMap<long, CCompositeSurfaceInfo> entries (caller: sort @ 0x00BEF5C0, landed).
// Element: see 0x00BED790. Shape: STLport 4.6 loop `__unguarded_linear_insert(i, T(*i), comp)`: the by-value argument
// is copy-constructed in its outgoing slot (implicit copy inline: key, vptr 0x012A8154, counted-pointer copy).
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};
typedef CKeyPairCompareLess<long, CCompositeSurfaceInfo> CSurfaceEntryLess;

namespace std {
template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);   // 0x00BED790

template <class RandomAccessIter, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
}   // namespace std

template void std::__unguarded_insertion_sort_aux(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess);