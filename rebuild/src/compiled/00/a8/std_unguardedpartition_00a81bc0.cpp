// std::__unguarded_partition(pair* first, pair* last, pair pivot, CKeyPairCompareLess comp) -- (pair<long, CCountedPointer<CNavConnectorNode> >) Hoare partition around the pivot's key: swap out-of-place pairs inward and return the split point
// Leaf: ??$__unguarded_partition@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@U12@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@std@@YIPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@PAU10@0U10@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81BC0, 103 bytes, `__fastcall` (first in ecx, last in edx; the 12-byte pivot and the empty comparator on the stack, `ret 10h`), returns first in eax.
// Identity (proven): std::__unguarded_partition<pair<long, CCountedPointer<CNavConnectorNode> >*, pair<...>, CKeyPairCompareLess<...> > (STLport 4.6 _algo.c; the PDB builds are Dinkumware); evidence: its only caller is the introsort loop 0x00A847C0 (J055), called by sort 0x00A84CF0, the sort of CNavQuadTree::ConnectorNodes (sorted at this+0x78 by CNavQuadTree::LinkToNeighbouringMaps 0x00A7A650): CVectorMap<long, CCountedPointer<CNavConnectorNode> > (J050 proves the element type from that sort's owner); it swaps through iter_swap 0x00A7FE70 and drops its by-value pivot's counted pointer at the end
// Label: the manifest's "Partition_Heap_12ByteElements" is invented (family J052: five byte-identical bodies, one per element type; retail has no identical-code folding).
// Callees: std::iter_swap<pair*, pair*>(a, b) 0x00A7FE70 (__fastcall, out of line), the pivot's count-block DeleteFunc and operator delete 0x00BFE9BC (~pivot).
// Body: `for (;;) { while (__comp(*__first, __pivot)) ++__first; --__last; while (__comp(__pivot, *__last)) --__last;
//   if (!(__first < __last)) return __first; iter_swap(__first, __last); ++__first; }`, the signed key compare only, then ~pivot.
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: J050 (agent950's landed CCountedPointer, key-only CKeyPairCompareLess).
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CNavConnectorNode;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
    bool operator()(const std::pair<K, V>& a, const K& key) const { return a.first < key; }
};
namespace std {
template <class _ForwardIter1, class _ForwardIter2>
void __fastcall iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2);   // 0x00A7FE70
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
typedef pair<long, CCountedPointer<CNavConnectorNode> > _Entry;
template _Entry* __fastcall __unguarded_partition<_Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > >(_Entry*, _Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> >);
}   // namespace std