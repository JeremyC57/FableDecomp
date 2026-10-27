// std::uninitialized_fill_n(CUnprovenEnum4_0085dc30* first, unsigned int n, const CUnprovenEnum4_0085dc30& x) -- STLport placement fill of n raw 4-byte slots with x, returns the end
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085DC30, 34 bytes, /Gr `__fastcall` (first ecx, n edx, &x on the stack), `ret 4`.
// Identity (descriptive): only caller the vector fill-insert 0x0085E6C0 (4-byte elements), element type not recoverable.
// Shape (family K076, representative 0x0040EA70): STLport 4.6 `uninitialized_fill_n` dispatching to
//   `__uninitialized_fill_n(first, n, x, __false_type())` (no __type_traits specialisation for the element), inlined:
//   unsigned n (`test edx,edx ; jbe`), the placement new's null test, one dword copy per slot (the landed K022/H-family fill).
inline void* operator new(unsigned int, void* where) { return where; }
enum CUnprovenEnum4_0085dc30 { };          // a 4-byte element (enum or one-word class) of an unidentified vector

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CUnprovenEnum4_0085dc30* __fastcall uninitialized_fill_n<CUnprovenEnum4_0085dc30*, unsigned int, CUnprovenEnum4_0085dc30>(CUnprovenEnum4_0085dc30*, unsigned int, const CUnprovenEnum4_0085dc30&);
} // namespace std