// std::__lower_bound(long* first, long* last, const long& val, less<long> comp, int*) -- STLport binary search for the first 4-byte signed integer that is not below val
// Leaf: ??$__lower_bound@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007771E0, 54 bytes, `ret 0Ch` (/Gr __fastcall: first in ecx -> eax, last in edx; &val,
//   the empty comparator and the null distance-type pointer on the stack).
// Identity: STLport 4.6 `__lower_bound` (`len = distance(first, last)`, `half = len >> 1`, `comp(*middle, val)` ->
//   `first = middle + 1; len = len - half - 1`, else `len = half`). Its only caller 0x00777716 is the binary_search front
//   end 0x00777700 (`i = lower_bound(first, last, val, less()); return i != last && !(val < *i);`, the empty comparator
//   built as a zeroed byte, the distance pointer pushed as 0). Element: a 4-byte signed scalar (`cmp [eax+edx*4],ecx ;
//   jge`, stride 4); the same code block holds the landed sort<long*, less<long> > family (__insertion_sort 0x00777920,
//   __make_heap 0x007772F0, sort_heap 0x00777EE0), so the instantiation is taken as <long*, long, less<long>, int>
//   (PROBABLE: long vs int/enum is not visible in the bytes). Catalog row was the placeholder `_gapscan::sub_007771e0`.
// Shape: `len` and `first` live in esi/eax; val is loaded once before the loop; the `len - half - 1` update is the
//   `or edi,-1 ; sub edi,edx ; add esi,edi` form.
namespace std {
template <class _Arg1, class _Arg2, class _Result> struct binary_function { };

template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp,
                                      _Distance*)
{
    _Distance __len = __last - __first;
    _Distance __half;
    _ForwardIter __middle;
    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        __middle += __half;
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}
template long* __fastcall __lower_bound(long*, long*, const long&, less<long>, int*);
}