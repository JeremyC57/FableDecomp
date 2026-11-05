// std::__unguarded_insertion_sort_aux(pair<long, CCountedPointer<CNavConnectorNode> >* first, pair<long, CCountedPointer<CNavConnectorNode> >* last, pair<long, CCountedPointer<CNavConnectorNode> >*, CKeyPairCompareLess comp) -- STLport final-insertion-sort pass over the vector-map entries: insert each element (a counted copy) into the already ordered prefix with the out-of-line __unguarded_linear_insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A80FF0, 64 bytes, /Gr __fastcall (first ecx, last edx; the _Tp* tag and the 4-byte
//   comparator on the stack), `ret 8`.
// Identity: its only callee is the landed std::__unguarded_linear_insert<pair<long, CCountedPointer<CNavConnectorNode> >*, ..> 0x00A7FF40 (agent953 J050: CNavQuadTree::ConnectorNodes).
// Callees: std::__unguarded_linear_insert 0x00A7FF40 (`__fastcall`: the element in ecx, the by-value pair built in the
//   argument area by CCountedPointer's copy constructor, the comparator copied ahead of it).
// Shape (family K013, representative 0x006B9ED0): STLport 4.6 _algo.c
//   `for (_RandomAccessIter __i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);`
//   with the comparator's operator() declaration-only (GOTCHAS 1474) and the pair's implicit copy constructor calling the
//   counted pointer's inline add-ref copy.
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
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A7FF40

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
typedef pair<long, CCountedPointer<CNavConnectorNode> > _Entry;
template void __fastcall __unguarded_insertion_sort_aux<_Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > >(_Entry*, _Entry*, _Entry*, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> >);
}   // namespace std