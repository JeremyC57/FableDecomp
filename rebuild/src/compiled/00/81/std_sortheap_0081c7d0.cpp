// std::sort_heap(pair<unsigned long, CSkeletalMorphs::CEntry>* first, pair<...>* last, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry> comp) -- STLport heap-sort tail: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@std@@YIXPAU?$pair@KVCEntry@CSkeletalMorphs@@@0@0V?$CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081c7d0, 179 bytes, /Gr `__fastcall` (first ecx, last edx, the empty comparator's stack word), `ret 4`.
// Identity (proven): its caller __partial_sort 0x0081D580 is the depth-limit fallback of __introsort_loop 0x0081DD90 (bound ??$__introsort_loop@PAU?$pair@KVCEntry@CSkeletalMorphs@@@std@@...CKeyPairCompareLess@KVCEntry@CSkeletalMorphs@@...), the sort of CSkeletalMorphs::Morphs (Ego_r.pdb CVectorMap<unsigned long, CSkeletalMorphs::CEntry>); 12-byte stride.
// Callee: __adjust_heap(first, 0, len, value, comp) @ 0x0081AC20 (this instantiation's own COMDAT, `__fastcall`).
// Body: STLport 4.6 `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap inline:
//   the value is copied out of *(last - 1) before `*(last - 1) = *first` (pair's user copy constructor; the assignment is the
//   implicit memberwise one).
// Shape (family L071): the landed sort_heap 0x0087E3B0 model with a 12-byte pair and an empty comparator.
class CDefString {                                         // Ego_r.pdb, 4 bytes
public:
    long TablePos;                                        // +0x00
};
class CSkeletalMorphs {
public:
    class CEntry {                                         // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                             // +0x00
        short Priority;                                   // +0x04
        bool SupressesLowerMorphs;                        // +0x06
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6's user copy constructor
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x0081AC20
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
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
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
template void __fastcall sort_heap<pair<unsigned long, CSkeletalMorphs::CEntry>*, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry> >(pair<unsigned long, CSkeletalMorphs::CEntry>*, pair<unsigned long, CSkeletalMorphs::CEntry>*, CKeyPairCompareLess<unsigned long, CSkeletalMorphs::CEntry>);
}   // namespace std