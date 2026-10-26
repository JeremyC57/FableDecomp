// std::sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp) -- (pair<long, long >*, CKeyPairCompareLess<long, long>) STLport introsort: for a non-empty range run __introsort_loop to depth 2*lg(n), then __final_insertion_sort
// Flags: /Gr
// Leaf: ??$sort@PAU?$pair@JJ@std@@V?$CKeyPairCompareLess@JJ@@@std@@YIXPAU?$pair@JJ@0@0V?$CKeyPairCompareLess@JJ@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FA41F, 66 bytes, __fastcall (/Gr TU: first ecx, last edx, the comparator by value on the stack), `ret 4`.
// Identity (proven): std::sort<std::pair<long, long >*, CKeyPairCompareLess<long, long> >; evidence: its only caller is CVectorMap<long, long, CKeyPairCompareLess<long, long> >::Sort 0x005FA496, called from 0x005F66D9 (the CTCInventoryAbilities member called by CycleAssignedExpressions that adds to this+0x1C8) on that map -- Ego_r.pdb ExpressionButtons at +0x208, 0x5C retail bytes after AbilityButtons (+0x16C, sorted by 0x005FA3DD) as the rebased layout puts them
// Label: the manifest's "std_make_heap_with_depth_adjust" is a behavioural / propagated label (family J067: five byte-identical sort bodies, each its own element and comparator).
// Callees: __introsort_loop(first, last, (value_type*)0, int depth_limit, comp) 0x005FA2F6, __final_insertion_sort(first, last, comp) 0x005FA07F
//   (both out of line; the bindings' ??$__introsort_loop spellings for this element where present).
// Shape: STLport 4.6 _algo.c `sort` verbatim in a size-optimised TU (the __lg loop enters at its test, the comparator is re-pushed
//   from its stack slot); __lg is inlined (`for (k = 0; n != 1; n >>= 1) ++k;`), (last - first) is the exact
//   pointer difference (`sar 3` for the 0x8-byte pair); the empty comparator travels as one pushed dword.
// Element: std::pair<long, long> (8 bytes), the element of Ego_r.pdb CTCInventoryAbilities::ExpressionButtons, CVectorMap<long, long, CKeyPairCompareLess<long, long> >
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
    typedef int difference_type;
};
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x005FA2F6
template <class _RandomAccessIter, class _Compare>
void __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x005FA07F
template <class _RandomAccessIter, class _Compare>
void sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0,
                         __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
}   // namespace std
template <class K, class V> class CKeyPairCompareLess { };   // Fable's vector-map key order (1 byte; its operator() lives in the out-of-line callees)
namespace std {
template void sort<pair<long, long >*, CKeyPairCompareLess<long, long> >(pair<long, long >*, pair<long, long >*, CKeyPairCompareLess<long, long>);
}