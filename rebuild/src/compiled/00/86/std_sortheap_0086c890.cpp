// std::sort_heap(pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>* first, pair<...>* last, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> comp) -- STLport heap-sort tail: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@std@@YIXPAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@0@0V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086c890, 179 bytes, /Gr `__fastcall` (first ecx, last edx, the empty comparator's stack word), `ret 4`.
// Identity (proven): its caller __partial_sort 0x0086D0C0 is the fallback of __introsort_loop 0x0086DA30, called from sort 0x0086DD20 (bound ??$sort@PAU?$pair@KVCTextureMorph@CRandomAppearanceMorph@@@std@@V?$CKeyPairCompareLess@KVCTextureMorph@CRandomAppearanceMorph@@@@...; caller CRandomAppearanceMorph::AddTextureMorph 0x0086BC10).
// Callee: __adjust_heap(first, 0, len, value, comp) @ 0x0086AFB0 (this instantiation's own COMDAT, `__fastcall`).
// Body: STLport 4.6 `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap inline:
//   the value is copied out of *(last - 1) before `*(last - 1) = *first` (pair's user copy constructor; the assignment is the
//   implicit memberwise one).
// Shape (family L071): the landed sort_heap 0x0087E3B0 model with a 12-byte pair and an empty comparator.
class CRandomAppearanceMorph {
public:
    class CTextureMorph {                                  // Ego_r.pdb, 8 bytes
    public:
        unsigned long NewTexture;                         // +0x00
        unsigned long GroupID;                            // +0x04
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
                              _Compare __comp);   // 0x0086AFB0
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
template void __fastcall sort_heap<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph> >(pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, pair<unsigned long, CRandomAppearanceMorph::CTextureMorph>*, CKeyPairCompareLess<unsigned long, CRandomAppearanceMorph::CTextureMorph>);
}   // namespace std