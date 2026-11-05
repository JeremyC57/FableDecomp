// std::__unguarded_linear_insert(pair<long,C3DMeshVolume>* last, pair<long,C3DMeshVolume> val, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort inner step over the navigation containment volumes: shift every entry with a larger key up one slot, drop the value into the gap, destroy the by-value copy
// Leaf: ??$__unguarded_linear_insert@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A703C0, 200 bytes (+8 int3), /Gr `__fastcall` (last in ecx; the
//   16-byte value and the empty comparator on the stack, `ret 14h`).
// Callees: CCharString::operator= 0x0099EFB0, C3DMeshVolume::operator= 0x00A32960 (the record assignment's out-of-line
//   COMDAT), the count block's DeleteFunc, operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0.
// Identity: the manifest's "std_copy_ComplexNumber_WithCleanup" is descriptive. The element is the 16-byte
//   pair<long, C3DMeshVolume> of CPhysicsMesh::NavigationContainmentVolumes (CVectorMap<long, C3DMeshVolume,
//   CKeyPairCompareLess<long, C3DMeshVolume> >, Ego_r.pdb) ordered by the SIGNED key (`jge`/`jl`); byte twin of the
//   landed unsigned-key 0x00A33250 (pair<unsigned long, C3DMeshVolume>, `jae`/`jb`), whose model this follows.
// Shape: STLport `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;`
//   -- the loop's pair assignment inlines the record assignment (string operator= call + the counted pointer's
//   ShareData: `Info != info` test, release with the count re-read, two stores, add-ref) while the final `*last = val`
//   calls the record's out-of-line COMDAT; then val dies (counted-pointer release, zero stores, string destructor).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

#include "engine/CCountedPointer.h"


class CContainmentVolume;

class C3DMeshVolume {                                   // 12 bytes (Ego_r.pdb)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)   // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

typedef pair<long, C3DMeshVolume> TNavEntry;
template void __fastcall __unguarded_linear_insert<TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume> >(TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume>);

}