// std::sort<ENavigatorType*>(ENavigatorType* first, ENavigatorType* last) -- STLport introsort: the depth-limited quicksort pass, then the final insertion sort
// Leaf: ??$sort@PAW4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006720B0, 77 bytes (+ int3 padding to 80), `__fastcall` (first in ecx, last in edx,
//   plain `ret`). Only caller: 0x0067175D.
//   Context: CThingCreatureDef::Transfer (the 0x0067175D..0x00671793 run) sorts the def's NavigatorTypes vector and
//   erases the duplicates: sort 0x006720B0, adjacent_find 0x00671BB0, __unique_copy 0x00671BE0 (landed), erase 0x00672070.
// Callees: __introsort_loop<ENavigatorType*, ENavigatorType, int, less<ENavigatorType> > @ 0x00672240 (`__fastcall`, value
//   pointer, depth limit and comparator on the stack), __final_insertion_sort @ 0x00672100.
// Label correction: the catalog "Vector_SortIntroSort" is descriptive.
// Shape: STLport verbatim `if (first != last) { __introsort_loop(first, last, (T*)0, __lg(last - first) * 2, __less((T*)0));
//   __final_insertion_sort(first, last, __less((T*)0)); }` -- the empty comparator temporaries share the `push ecx` slot,
//   passed by value (its garbage dword pushed; less<> has a user ctor, else each temporary gets a `mov byte [esp+8],0`),
//   __lg inline (`sar eax,1 ; inc ecx`). No pragma.
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)
namespace std {
template <class _Tp> struct less {
    less() { }                                  // a user ctor: the empty temporary emits no zero byte store
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00672240
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00672100
template <class _RandomAccessIter>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, __value_type(__first), __lg(__last - __first) * 2, __less(__value_type(__first)));
        __final_insertion_sort(__first, __last, __less(__value_type(__first)));
    }
}
template void __fastcall sort(ENavigatorType*, ENavigatorType*);
}