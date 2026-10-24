// std::__unguarded_insertion_sort<ENavigatorType*,less<ENavigatorType> >(ENavigatorType* first, ENavigatorType* last, less<ENavigatorType> comp) -- STLport unguarded insertion sort: shift each element left past its larger predecessors (a smaller element is known to sit before first)
// Leaf: ??$__unguarded_insertion_sort@PAW4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00672180, 50 bytes (+ int3 padding to 64), `__fastcall` (first in ecx, last in
//   edx, the comparator on the stack, `ret 4`). Caller: __final_insertion_sort 0x00672100 (call at 0x00672143).
//   Context: CThingCreatureDef::Transfer (the 0x0067175D..0x00671793 run) sorts the def's NavigatorTypes vector and
//   erases the duplicates: sort 0x006720B0, adjacent_find 0x00671BB0, __unique_copy 0x00671BE0 (landed), erase 0x00672070.
// Label correction: the catalog "_Copy_impl<CComplexNumber*,CComplexNumber*>" is a propagated name (no copy, 4-byte
//   signed elements); this is STLport __unguarded_insertion_sort with __unguarded_insertion_sort_aux and
//   __unguarded_linear_insert inline.
// Shape: STLport verbatim `__unguarded_insertion_sort_aux(first, last, (T*)0, comp)` -> `for (i = first; i != last; ++i)
//   __unguarded_linear_insert(i, T(*i), comp);` -> `next = last; --next; while (comp(val, *next)) { *last = *next;
//   last = next; --next; } *last = val;` (signed `jge`/`jl`); aux and the linear insert are __forceinline (plain inline leaves the aux call). No pragma.
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)
namespace std {
template <class _Tp> struct less {
    less() { }                                  // a user ctor: the empty temporary emits no zero byte store
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, __value_type(__first), __comp);
}
template void __fastcall __unguarded_insertion_sort(ENavigatorType*, ENavigatorType*, less<ENavigatorType>);
}