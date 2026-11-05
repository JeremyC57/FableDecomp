// std::__linear_insert(CSurfaceEntry* first, CSurfaceEntry* last, CSurfaceEntry val, CSurfaceEntryLess comp) -- insertion step: a new minimum shifts [first, last) up one slot and lands at first, anything else takes the unguarded linear insert
// Leaf: ??$__linear_insert@PAU?$pair@JVCCompositeSurfaceInfo@@@std@@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCPPointerInfo members DeleteFunction->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE8F0, 160 bytes, `ret 14h` (the 16-byte value + the empty comparator).
// Callees: __copy_backward @ 0x00BED9A0 (landed), the entry's implicit operator= @ 0x00BED640 (out of line here),
// __unguarded_linear_insert @ 0x00BED790 (this chunk), operator delete @ 0x00BFE9BC, ~CBaseClass @ 0x0099A2E0.
// Label: the manifest's "Std_Vector_InsertWithMove" is descriptive; caller __insertion_sort @ 0x00BEECD0 (this chunk).
// Element: see 0x00BED790. Shape: STLport 4.6 __linear_insert; the random_access_iterator_tag temporary of the inline
// copy_backward is homed in the dead comparator slot.
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
template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                   const random_access_iterator_tag&, Distance*);   // 0x00BED9A0

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);   // 0x00BED790

template <class RandomAccessIter, class T, class Compare>
void __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}
}   // namespace std

template void std::__linear_insert(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry, CSurfaceEntryLess);