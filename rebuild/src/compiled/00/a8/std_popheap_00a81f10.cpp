// std::__pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result, _Tp val, _Compare comp, _Distance*) -- (pair<long, CCountedPointer<CNavConnectorNode> > under CKeyPairCompareLess) move the heap top to result, then sift the saved value into the hole
// Leaf: ??$__pop_heap@PAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@std@@U12@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@H@std@@YIXPAU?$pair@JV?$CCountedPointer@VCNavConnectorNode@@@@@0@00U10@V?$CKeyPairCompareLess@JV?$CCountedPointer@VCNavConnectorNode@@@@@@PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81F10, 175 bytes, /Gr `__fastcall` (first in ecx, last in edx; result, the
//   12-byte value, the empty comparator and the dead distance tag on the stack), `ret 18h`.
// Identity (proven): std::__pop_heap<pair<long, CCountedPointer<CNavConnectorNode> >*, pair<...>, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> >, int>; evidence: reached from the sort 0x00A84CF0 whose element H047 proves (CNavQuadTree::ConnectorNodes (LinkToNeighbouringMaps); swap 0x00A7FE70) by direct calls 0x00A84CF0 -> 0x00A847C0 -> 0x00A83E80 -> 0x00A81F10 (sort -> introsort_loop -> partial_sort -> this); its four callers 0x00A821A8, 0x00A82D18, 0x00A83653, 0x00A83EC7 are that TU's heap code.
// Label: the manifest's "_Pop_heap<std::pair<CCharString,CCountedPointer<CEntityScriptBindingBase>_>*,int,std::pair<class_CCharString,class_CCountedPointer<class_CEntityScriptBindingBase>_>,CCompareString<CCountedPointer<CEntityScriptBindingBase>_>_>" is propagated (family H048: six byte-identical bodies, each its own COMDAT; retail has
//   no identical-code folding, so each address is one element type).
// Callees: __adjust_heap 0x00A81170 (__fastcall: first, hole 0, len, the value copied into its argument slot, comp) and
//   operator delete 0x00BFE9BC (the releases in the inline pair assignment and in the by-value parameter's destructor).
// Shape: STLport 4.6 _heap.h __pop_heap verbatim: `*result = *first; __adjust_heap(first, 0, last - first, val, comp);`
//   (len by the 0x2AAAAAAB reciprocal, 12-byte stride); the pair's implicit assignment inlines CCountedPointer::operator=
//   (landed 0x00448380 body), and the callee-destroyed `val` is released after the call.
#include "engine/CCountedPointer.h"
class CNavConnectorNode;

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                             // +0x0
    _T2 second;                                            // +0x4
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
}
template <class K, class V> class CKeyPairCompareLess {    // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);            // 0x00A81170
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
typedef pair<long, CCountedPointer<CNavConnectorNode> > _Elem;
typedef CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > _Cmp;
template void __fastcall __pop_heap<_Elem*, _Elem, _Cmp, int>(_Elem*, _Elem*, _Elem*, _Elem, _Cmp, int*);
} // namespace std