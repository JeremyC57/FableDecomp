// std::__final_insertion_sort<ENavigatorType*,less<ENavigatorType> >(ENavigatorType* first, ENavigatorType* last, less<ENavigatorType> comp) -- STLport sort finish: guarded insertion sort of the first 16, unguarded of the rest (or guarded of all when 16 or fewer)
// Leaf: ??$__final_insertion_sort@PAW4ENavigatorType@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00672100, 123 bytes (+ int3 padding to 128), `__fastcall` (first in ecx, last in
//   edx, the comparator on the stack, `ret 4`). Only caller: sort 0x006720B0.
//   Context: CThingCreatureDef::Transfer (the 0x0067175D..0x00671793 run) sorts the def's NavigatorTypes vector and
//   erases the duplicates: sort 0x006720B0, adjacent_find 0x00671BB0, __unique_copy 0x00671BE0 (landed), erase 0x00672070.
// Callees: __linear_insert<ENavigatorType*, ENavigatorType, less<ENavigatorType> > @ 0x006721C0 (`__fastcall`, value and
//   comparator on the stack), __unguarded_insertion_sort @ 0x00672180.
// Label correction: the catalog "Vector_InsertSorted" is descriptive.
// Shape: STLport verbatim `if (last - first > __stl_threshold) { __insertion_sort(first, first + 16, (T*)0, comp);
//   __unguarded_insertion_sort(first + 16, last, comp); } else __insertion_sort(first, last, (T*)0, comp);` with
//   __insertion_sort __forceinline (plain inline stays a call) twice (`for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);`). No pragma.
enum ENavigatorType { NAVIGATOR_TYPE_0 };                  // 4-byte enum (signed compares)
namespace std {
template <class _Tp> struct less {
    less() { }                                  // a user ctor: the empty temporary emits no zero byte store
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x006721C0
template <class _RandomAccessIter, class _Compare>
void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);  // 0x00672180
template <class _RandomAccessIter, class _Tp, class _Compare>
__forceinline void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert<_RandomAccessIter, _Tp, _Compare>(__first, __i, *__i, __comp);
}
const int __stl_threshold = 16;
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __value_type(__first), __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __value_type(__first), __comp);
}
template void __fastcall __final_insertion_sort(ENavigatorType*, ENavigatorType*, less<ENavigatorType>);
}