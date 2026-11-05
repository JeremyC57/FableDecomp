// std::__make_heap(pair<long,C3DMeshVolume>* first, pair<long,C3DMeshVolume>* last, CKeyPairCompareLess<long,C3DMeshVolume> comp, pair<long,C3DMeshVolume>*, int*) -- build a heap over the navigation containment volumes (signed key order) by sifting down every parent from the last one to the root
// Leaf: ??$__make_heap@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A71CA0, 116 bytes, /Gr `__fastcall` (first in ecx, last in edx; the
//   empty comparator, the value-type and distance-type tag pointers on the stack, `ret 0Ch`).
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the by-value element copy's volume name),
//   std::__adjust_heap 0x00A71220 (__fastcall first/hole, then len, the 16-byte value, the comparator).
// Identity: the manifest's "Heap_BuildHeap_LongKey" is descriptive. 16-byte pair<long, C3DMeshVolume> entries of
//   CPhysicsMesh::NavigationContainmentVolumes (CKeyPairCompareLess<long, C3DMeshVolume>).
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- first
//   is kept in the `push ecx` frame word; the by-value copy is built in the argument area (key word, the name's copy
//   constructor, the counted pointer's two words and AddRef) through a pointer at element + 8.
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

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A71220

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

typedef pair<long, C3DMeshVolume> TNavEntry;
template void __fastcall __make_heap<TNavEntry*, CKeyPairCompareLess<long, C3DMeshVolume>, TNavEntry, int>(TNavEntry*, TNavEntry*, CKeyPairCompareLess<long, C3DMeshVolume>, TNavEntry*, int*);

}