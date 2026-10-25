// std::less<pair<long,long>>::operator()(const pair<long,long>& x, const pair<long,long>& y) -- lexicographic order on long pairs: first, then second
// Leaf: ??R?$less@U?$pair@JJ@std@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD11A0, 42 bytes, `__fastcall` (the empty comparator in ecx, unused),
//   `ret 8`. No callees.
// Identity: STLport `less<_Tp>::operator()` over `pair<long,long>` (manifest signature
//   `less<pair<long,long>>*, pair<long,long>*, pair<long,long>*`), emitted out of line and shared by the sorts at
//   0x00AD56E9.. and 0x00B2F2F9.. (identical COMDAT folded by the linker).
// Shape: STLport's pair `operator<` -- `x.first < y.first || (!(y.first < x.first) && x.second < y.second)`: one
//   compare of the first members feeds both `jg` (true) and `jl` (false); the second members compare `jl`; the bool is
//   materialised as `xor eax,eax` / `mov eax,1`.
namespace std {
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first ||
           (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template struct less<pair<long, long> >;
}