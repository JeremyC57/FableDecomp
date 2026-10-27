// std::sort_heap(pair<EExpressionType, CExpressionSet::CEntry>* first, pair<...>* last, CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> comp) -- STLport heap-sort tail: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@std@@YIXPAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@0@0V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086fb40, 179 bytes, /Gr `__fastcall` (first ecx, last edx, the empty comparator's stack word), `ret 4`.
// Identity (proven): bound (reloc_bindings) ??$sort_heap@PAU?$pair@W4EExpressionType@@VCEntry@CExpressionSet@@@std@@V?$CKeyPairCompareLess@W4EExpressionType@@VCEntry@CExpressionSet@@@@@std@@YIX...@Z from its caller, the landed __partial_sort 0x0086FEB0; callee the landed __adjust_heap 0x0086F450 of the same instantiation.
// Callee: __adjust_heap(first, 0, len, value, comp) @ 0x0086F450 (this instantiation's own COMDAT, `__fastcall`).
// Body: STLport 4.6 `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap inline:
//   the value is copied out of *(last - 1) before `*(last - 1) = *first` (pair's user copy constructor; the assignment is the
//   implicit memberwise one).
// Shape (family L071): the landed sort_heap 0x0087E3B0 model with a 12-byte pair and an empty comparator.
enum EExpressionType { };
class CExpressionSet {
public:
    class CEntry {                                         // Ego_r.pdb, 8 bytes
    public:
        EExpressionType Type;                             // +0x00
        long AnimIndex;                                   // +0x04
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
                              _Compare __comp);   // 0x0086F450
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
template void __fastcall sort_heap<pair<EExpressionType, CExpressionSet::CEntry>*, CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry> >(pair<EExpressionType, CExpressionSet::CEntry>*, pair<EExpressionType, CExpressionSet::CEntry>*, CKeyPairCompareLess<EExpressionType, CExpressionSet::CEntry>);
}   // namespace std