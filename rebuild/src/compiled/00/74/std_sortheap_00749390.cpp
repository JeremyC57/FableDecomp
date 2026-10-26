// std::sort_heap(const CThing** first, const CThing** last, CCompare_FurthestFromInitPos2D comp) -- STLport heap-sort tail of the furthest-from-initial-position thing sort: pop the heap root to the back until one element remains
// Leaf: ??$sort_heap@PAPBVCThing@@VCCompare_FurthestFromInitPos2D@?A0x2068fc7d@@@std@@YIXPAPBVCThing@@0VCCompare_FurthestFromInitPos2D@?A0x2068fc7d@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749390, 78 bytes (+ int3 padding to 80), `__fastcall` (/Gr algorithm TU: first in
//   ecx, last in edx, the 4-byte comparator on the stack), `ret 4`.
// Identity (proven): _RandomAccessIterator = const CThing**, _Compare = CCompare_FurthestFromInitPos2D. its callee 0x00748C90 is the landed __adjust_heap over things with CCompare_FurthestFromInitPos2D (operator() 0x007483E0) and its caller the partial_sort 0x00749530 of the same instantiation; the Ego_r.pdb name set spells it ??$sort_heap@PAPBVCThing@@VCCompare_FurthestFromInitPos2D@?A0xae0dd725@@... (const CThing*, an anonymous-namespace empty comparator); the catalog's non-template ?sort_heap@std@@YIXPAPAUCThing@@0UCCompare_FurthestFromInitPos2D@@@Z is the landed caller's model.
// Label: the manifest's "_Sort_heap<long*>" is propagated across byte-identical bodies (family H008: seven rows that differ only in
//   their call displacement; retail has no identical-code folding, so each address is one instantiation).
// Callee: __adjust_heap(first, 0, len, val, comp) @ 0x00748C90 (this instantiation's own COMDAT, __fastcall `ret 0Ch`).
// Shape: STLport 4.6 `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap
//   inline (the landed sort_heap 0x0097B2C0 model): `*(last - 1)` is read into the value before `*(last - 1) = *first`, the
//   comparator word is cached in ebx and pushed straight from it.
class CThing;
namespace {
class CCompare_FurthestFromInitPos2D {              // Ego_r.pdb: empty, anonymous namespace (?A0xae0dd725)
public:
    bool operator()(const CThing* a, const CThing* b) const;   // 0x007483E0
};
}
typedef CCompare_FurthestFromInitPos2D TComp;
namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);   // 0x00748C90

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
template void __fastcall sort_heap<const CThing**, TComp>(const CThing**, const CThing**, TComp);
}   // namespace std