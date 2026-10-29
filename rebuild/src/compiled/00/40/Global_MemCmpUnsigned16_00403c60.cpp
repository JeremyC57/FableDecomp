// std::__char_traits_base<wchar_t, wint_t>::compare(const wchar_t* lhs, const wchar_t* rhs, size_t count) -- compare two runs of 16-bit characters, memcmp-style
// Leaf: ?compare@?$__char_traits_base@GG@std@@SAHPBG0I@Z (STLport 4.6 char_traits.h: char_traits<wchar_t> inherits the static
//   __cdecl compare from __char_traits_base<wchar_t, wint_t>; wchar_t == wint_t == unsigned short without /Zc:wchar_t;
//   catalogued as MemCmp_Unsigned16)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403C60, 75 bytes. Callees: none.
//
// Element-wise comparison for wide strings: walks `count` 16-bit units and returns -1 / 1 at the first
// pair that differs (ordered by unsigned code unit) or 0 when the runs are equal. This is the comparison
// behind CWideString equality and ordering, so it deliberately compares code units, not bytes, and
// never stops at a NUL; the caller decides the length. A zero count compares equal.
//
// Matching: single indexed loop with the compare written as `!=` then `<`, which gives the
// `jae`/`sbb`-free two-branch body; `index` is the loop variable rather than advancing pointers.
namespace std {
template <class _CharT, class _IntT> class __char_traits_base {
public:
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);
};
}

template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(
    const unsigned short* lhs, const unsigned short* rhs, unsigned int count)
{
    unsigned int index;
    for (index = 0; index < count; ++index)
    {
        // First difference decides the order.
        if (lhs[index] != rhs[index])
            return lhs[index] < rhs[index] ? -1 : 1;
    }
    return 0;
}
