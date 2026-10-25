// std::sort_heap<pair<unsigned __int64,CANavQuadTreeNode*>*, CKeyPairCompareLess<unsigned __int64,CANavQuadTreeNode*> >(first, last, comp) -- STLport
// sort_heap over the 16-byte CNavQuadTree::SwitchableNodes (CVectorMap<unsigned __int64,CANavQuadTreeNode*>) entries: pop the root to the back until one element remains
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A81E80, length 141 bytes (+ int3 padding), `ret 4`.
// Name: Ego_r.pdb 0x00BC3120 is the same instantiation under Dinkumware
//   (`??$_Sort_heap@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@std@@YIX...`, /Gr); retail is STLport 4.6,
//   whose spelling is sort_heap -> pop_heap -> __pop_heap_aux -> __pop_heap (all inline) -> __adjust_heap. Was catalogued
//   `std::_Sort_heap` (Dinkumware) over a descriptive element; the manifest's
//   `_Sort_heap<pair<long,CRandomAppearanceMorph::CSkeletalMorph>*,...>` is propagated. Twin of the bool-valued 0x00A81D90.
// Callees: __adjust_heap @ 0x00A7F1A0 (out of line, __fastcall: first in ecx, holeIndex 0 in edx, `ret 18h`; landed under the
//   Dinkumware name _Adjust_heap).
// Shape: /Gr TU (explicit __fastcall here). The `_Tp(*(__last - 1))` temporary keeps the low key dword in ecx and spills
// the high dword and the value to the frame (STLport pair's memberwise copy constructor skips the padding);
// `*__result = *__first` is the implicit assignment (four dword moves, padding included), and the value argument gets
// the two key dwords and the value.
// Leaf: ??$sort_heap@PAU?$pair@_KPAVCANavQuadTreeNode@@@std@@V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@std@@YIXPAU?$pair@_KPAVCANavQuadTreeNode@@@0@0V?$CKeyPairCompareLess@_KPAVCANavQuadTreeNode@@@@@Z
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x08
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

class CANavQuadTreeNode;

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00A7F1A0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                       _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

typedef pair<unsigned __int64, CANavQuadTreeNode*> _CEntry;
template void __fastcall sort_heap(_CEntry*, _CEntry*, CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*>);
}
