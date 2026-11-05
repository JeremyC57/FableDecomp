// std::__unguarded_linear_insert(long* last, pair val, CKeyPairCompareLess comp) -- (pair<long, CCountedPointer<CContainmentVolume> >) shift each element whose key is above val's one slot up, then store val in the hole
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@std@@U12@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@std@@YIXPAU?$pair@JV?$CCountedPointer@VCContainmentVolume@@@@@0@U10@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCContainmentVolume@@@@@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008251B0, 186 bytes, `__fastcall` (last in ecx; the 12-byte val and the empty 4-byte comparator on the stack, `ret 10h`).
// Identity (proven): std::__unguarded_linear_insert<pair<long, CCountedPointer<CContainmentVolume> >*, pair<...>, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > > (FableWin.pdb name set); evidence: its callers, the unguarded insertion sort 0x00825B40 and the linear insert 0x00826DD0, sit under the landed __insertion_sort 0x008270E0 (pair<long, CCountedPointer<CContainmentVolume> >) and the final insertion sort 0x00827350 of sort 0x00827710. CMap::InitCachedVolumes 0x00821850 calls them on the cached-volume map: Ego_r.pdb CCachedVolumes::NavigationContainmentVolumes; the key compare is signed (jl/jge), and Ego_r.pdb has exactly five CVectorMap<signed key, CCountedPointer<X> > members (CGamePlayerInterface::Controls, CCachedVolumes::NavigationContainmentVolumes, CRegionConnectionGraph::GraphNodes, CNavQuadTree::ConnectorNodes, CParticleEmitterDatabase::Emitters), one per J050 row
// Label: the manifest's "StdVector_AssignRange" is invented/propagated (family J050: five byte-identical bodies, one per element type; retail has no identical-code folding).
// Callees: the DeleteFunc of the count block (`call [Info+4]`, Data in ecx) and operator delete 0x00BFE9BC, both from the inlined CCountedPointer assignment and destructor.
// Body: STLport 4.6 _algo.c `__next = __last; --__next; while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; } *__last = __val;`,
//   the comparator comparing the keys only (signed), each pair assignment the implicit one (key, then CCountedPointer::operator= = ShareData), then ~val.
// Layout: pair<K, CCountedPointer<V> > is 12 bytes {first +0, second.Data +4, second.Info +8}; CCPPointerInfo {RefCount +0, DeleteFunc +4, Data +8}.
// Model: agent950's landed CCountedPointer (0x00A99110: ReleaseData / ShareData) and the landed 0x00448910 caller model.
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CContainmentVolume;
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
typedef pair<long, CCountedPointer<CContainmentVolume> > _Entry;
template void __fastcall __unguarded_linear_insert<_Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> > >(_Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<CContainmentVolume> >);
}   // namespace std