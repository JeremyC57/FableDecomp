// std::uninitialized_fill_n(CHeroLogBook::CHeroLogBookEntry::ECategory* first, unsigned int n, const CHeroLogBook::CHeroLogBookEntry::ECategory& x) -- STLport placement fill of n raw 4-byte slots with x, returns the end
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006132B0, 34 bytes, /Gr `__fastcall` (first ecx, n edx, &x on the stack), `ret 4`.
// Identity (probable): only caller the vector fill-insert 0x00618AD0, whose sibling insert-overflow 0x00618200 is bound ?_M_insert_overflow@?$vector@W4ECategory@CHeroLogBookEntry@CHeroLogBook@@...; so the element is CHeroLogBook::CHeroLogBookEntry::ECategory.
// Shape (family K076, representative 0x0040EA70): STLport 4.6 `uninitialized_fill_n` dispatching to
//   `__uninitialized_fill_n(first, n, x, __false_type())` (no __type_traits specialisation for the element), inlined:
//   unsigned n (`test edx,edx ; jbe`), the placement new's null test, one dword copy per slot (the landed K022/H-family fill).
inline void* operator new(unsigned int, void* where) { return where; }
class CHeroLogBook {
public:
    class CHeroLogBookEntry {
    public:
        enum ECategory { };                              // Ego_r.pdb (enumerators not needed)
    };
};

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
template CHeroLogBook::CHeroLogBookEntry::ECategory* __fastcall uninitialized_fill_n<CHeroLogBook::CHeroLogBookEntry::ECategory*, unsigned int, CHeroLogBook::CHeroLogBookEntry::ECategory>(CHeroLogBook::CHeroLogBookEntry::ECategory*, unsigned int, const CHeroLogBook::CHeroLogBookEntry::ECategory&);
} // namespace std