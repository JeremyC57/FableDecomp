// std::__linear_insert(pair<long, CCountedPointer<CNavConnectorNode> >* first, pair<long, CCountedPointer<CNavConnectorNode> >* last, pair<long, CCountedPointer<CNavConnectorNode> > val, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > comp) -- SGI insertion-sort step over the connector map's (long id, counted node) pairs: a value below the front shifts the run up one slot and lands at the front, otherwise the unguarded backward scan places it
// Leaf: ??$__linear_insert@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A834B0, 113 bytes, `ret 10h`: /Gr `__fastcall` (first in ecx -> esi, last
//   in edx; the 12-byte value and the empty comparator on the stack).
// Identity: the only caller 0x00A83D57 is the __insertion_sort of CVectorMap<long, CCountedPointer<CNavConnectorNode> >
//   (the CNavQuadTree::ConnectorNodes map, see DeallocateMemory 0x00A83FD0); the callees are the landed
//   __copy_backward 0x00A802A0 and pair operator= 0x00A7FAF0 of the same 12-byte pair. The twin of the line-list map's
//   __linear_insert 0x00A843F0. The catalog's "Vector_InsertOrErase_CountedPointer" is descriptive.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, int*) 0x00A802A0, pair::operator= 0x00A7FAF0,
//   __unguarded_linear_insert(last, value, comp) 0x00A7FF40 (`__fastcall`), operator delete 0x00BFE9BC.
// Shape: STLport 4.6 `if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1); *__first = __val; } else
//   __unguarded_linear_insert(__last, __val, __comp);` -- the signed key compare inline, copy_backward's empty tag homed in
//   the dead comparator slot, the by-value copy for the unguarded scan built inline in its argument slot (key, object,
//   block, count bump), then `__val`'s counted pointer released on both exits (`if (Info && --Info->RefCount == 0)`).
//   `*__first = __val` is the pair's IMPLICIT assignment (STLport pair declares none; CCountedPointer has an inline
//   operator=): VC7.1 emits it as the out-of-line COMDAT 0x00A7FAF0 and calls it, knowing its body, so `__val` is not
//   address-exposed and its count block is loaded per arm with `__first` kept in esi. A user-declared noinline pair
//   operator= instead hoists that load to the entry (first in edi, an extra push/pop); a declaration only re-reads it
//   around every call.
void __cdecl operator delete(void* block);              // 0x00BFE9BC

#include "engine/CCountedPointer.h"


class CNavConnectorNode;

namespace std {
template <class _T1, class _T2> struct pair {           // STLport 4.6 pair (user copy constructor, implicit assignment)
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}

template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const random_access_iterator_tag&,
                                               _Distance*);                                  // 0x00A802A0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A7FF40

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    } else
        __unguarded_linear_insert(__last, __val, __comp);
}
}

typedef std::pair<long, CCountedPointer<CNavConnectorNode> > SConnectorEntry;   // 12 bytes
template void __fastcall std::__linear_insert(SConnectorEntry* __first, SConnectorEntry* __last, SConnectorEntry __val,
                                               CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > __comp);