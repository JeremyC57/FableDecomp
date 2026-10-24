// std::lower_bound(first, last, const pair<long,float>& val) -- STLport lower_bound over a sorted (theme id, weight) array, forwarding to __lower_bound with less<> and the distance type
// Leaf: ??$lower_bound@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B5A50, length 27 bytes, /Gr __fastcall: ecx = first, edx = last
//   passed straight through, val on the stack, `ret 4`.
// Callees: std::__lower_bound<pair<long,float>*, pair<long,float>, less<pair<long,float> >, int> @ 0x006B5890
//   (__fastcall `ret 0Ch`: &val, the empty comparator by value, (int*)0).
// Identity: the catalog's "operator_delete" is a propagated label; the only caller is 0x006B3F1F in
//   CEnvironment::SetWeightedThemes, searching the function-static `ThemeWeights` vector (0x013BA8EC) of
//   pair<long, float> before a vector::insert (0x006B5ED0). 0x006B5890's compare is the STLport pair operator<
//   (first by `cmp`/`jg`/`jl`, then second by `fcom`).
// Shape: `push ecx` frames the comparator temporary; `__less((_Tp*)0)` returns less<_Tp>() and VC zero-fills
//   that one byte (`mov byte ptr [esp+4],0`) before copying it into the by-value argument slot; the
//   distance type is the `push 0`. The zero fill needs less<> WITHOUT the binary_function base: with the
//   (empty) base the class is no longer POD, `less<_Tp>()` is not zero-initialised and VC passes a dead
//   argument word instead (DIFFER 20v27).
namespace std {
typedef int ptrdiff_t;
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T1, class T2>
inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> inline less<T> __less(T*) { return less<T>(); }

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val,
                                      _Compare __comp, _Distance*);      // 0x006B5890

template <class _ForwardIter, class _Tp>
_ForwardIter __fastcall lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    return __lower_bound(__first, __last, __val, __less((_Tp*)0), (ptrdiff_t*)0);
}

template pair<long, float>* __fastcall lower_bound(pair<long, float>*, pair<long, float>*, const pair<long, float>&);
}