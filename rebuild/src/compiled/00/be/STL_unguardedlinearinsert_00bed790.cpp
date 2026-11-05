// std::__unguarded_linear_insert(CSurfaceEntry* last, CSurfaceEntry val, CSurfaceEntryLess comp) -- shift larger-keyed (key, CCompositeSurfaceInfo) entries up one slot until val's place is found (no lower bound check), store val there
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BED790, 225 bytes, `ret 14h` (the 16-byte value + the empty comparator).
// Callees: operator delete @ 0x00BFE9BC (count blocks), ~CBaseClass @ 0x0099A2E0 (the by-value value's base).
// Label: the manifest's "CNavigationPosition_CopyRange" is propagated; this is the STLport sort instantiation of
// CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > (Ego_r.pdb name set).
// Element: std::pair<long, CCompositeSurfaceInfo> (16 bytes): key, then CCompositeSurfaceInfo {vptr 0x012A8154,
// CCountedPointer<CCompositeSurfaceCache> Cache}; the implicit operator= is the key store plus the counted-pointer
// assignment (vptr untouched), the by-value value's implicit destructor releases Cache and calls ~CBaseClass.
// Shape: STLport 4.6 __unguarded_linear_insert; signed key compare inline (`jge`/`jl`).
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
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}
}   // namespace std

template void std::__unguarded_linear_insert(CSurfaceEntry*, CSurfaceEntry, CSurfaceEntryLess);