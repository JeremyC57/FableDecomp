// std::sort_heap(unsigned long* first, unsigned long* last, CompareDataOffset comp) -- STLport heap-sort tail of a bank's entry indices by data offset: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@PAKUCompareDataOffset@@@std@@YIXPAK0UCompareDataOffset@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D42B0, 78 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx, the 4-byte comparator on the stack), `ret 4`.
// Identity (proven): _RandomAccessIterator = unsigned long*, _Compare = CompareDataOffset. its callee 0x009D3750 is the landed __adjust_heap over entry indices with CBankFile::CompareDataOffset and its caller the __partial_sort 0x009D4620 of the same sort (whose __introsort_loop 0x009D48C0 is landed); catalog binding ??$sort_heap@PAKUCompareDataOffset@@@std@@..., which this source defines.
// Label: the manifest's "_Sort_heap<long*>" is propagated across byte-identical bodies (family H008: seven rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, 0, len, val, comp) @ 0x009D3750 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap
//   inline (the landed sort_heap 0x0097B2C0 model): `*(last - 1)` is read into the value before `*(last - 1) = *first`, the
//   comparator word is cached in ebx and pushed straight from it.
class CBankFile;
// CBankFile::CompareDataOffset (Ego_r.pdb, nested, 4 bytes), at namespace scope as in the landed relatives (catalog spelling).
struct CompareDataOffset {
    CBankFile* BankFile;                            // +0x00
    bool operator()(unsigned long a, unsigned long b);   // 0x009CC640
};
typedef CompareDataOffset TComp;
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x009D3750

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
template void __fastcall sort_heap<unsigned long*, TComp>(unsigned long*, unsigned long*, TComp);
}   // namespace std