// std::fill_n(unsigned __int64* first, unsigned int n, const unsigned __int64& value) -- assign value to n consecutive 64-bit elements starting at first and return the end of the filled range
// Leaf: ??$fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006149B0, 36 bytes, __fastcall (first ecx, n edx; the value pointer on the stack, `ret 4`).
// Callers: one, 0x00618086 (a vector<unsigned __int64> insert path at 0x00617FD0: STLport's __uninitialized_fill_n over a
//   trivially-copyable element forwards to fill_n, and the 8-byte stride with two dword moves and no null test is the
//   scalar assignment of a 64-bit integer).
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated; the body is STLport's fill_n (no placement new, one
//   stack argument).
// Shape: STLport 4.6 `for (; __n > 0; --__n, ++__first) *__first = __val; return __first;` -- the value pointer is loaded
//   once, the loop entered by a jump over its alignment padding.
namespace std {
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template unsigned __int64* __fastcall fill_n(unsigned __int64*, unsigned int, const unsigned __int64&);
}