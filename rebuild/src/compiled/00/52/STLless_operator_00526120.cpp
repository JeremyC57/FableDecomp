// std::less::operator()(const unsigned __int64& x, const unsigned __int64& y) -- less<unsigned __int64>: strict weak order on 64-bit unsigned keys: x < y
// Leaf: ??R?$less@_K@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00526120, 37 bytes (+ int3 padding), `__fastcall` (the empty
//   comparator in ecx, unused), `ret 8`. No callees.
// Identity: STLport `less<_Tp>::operator()` (`return __x < __y;`) instantiated for unsigned __int64 and emitted out of
//   line as its own COMDAT (callers 0x0052A49C, 0x0082FAFC pass the two key addresses). The catalog row name
//   `less<unsigned___int64>::operator()` is correct; STLport lives in namespace std.
// Shape: the 64-bit compare tests the high words first (`ja` false / `jb` true), then the low words unsigned (`jae`
//   false); the bool result is materialised as a full `mov eax,1` / `xor eax,eax` on the two exits.
namespace std {
template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1   first_argument_type;
    typedef _Arg2   second_argument_type;
    typedef _Result result_type;
};

template <class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template struct less<unsigned __int64>;
}