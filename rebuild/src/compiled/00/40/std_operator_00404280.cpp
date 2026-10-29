// std::operator==(const basic_string<wchar_t>&, const wchar_t*) -- STLport 4.6 out-of-line instantiation for the wide
// string (wchar_t == unsigned short under VC7.1 without /Zc:wchar_t, mangled G).
// Leaf: ??$?8GV?$char_traits@G@std@@V?$allocator@G@1@@std@@YA_NABV?$basic_string@GV?$char_traits@G@std@@V?$allocator@G@2@@0@PBG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00404280, 67 bytes. Callee: __char_traits_base<wchar_t, wint_t>::compare @ 0x00403C60.
// Name: STLport 4.6.2 stl/_string.h operator==(const basic_string<_CharT,_Traits,_Alloc>&, const _CharT*): length of the
//   C string (char_traits::length, inlined), size compare, then _Traits::compare; the landed caller 0x0049EF50 spells the
//   class templates V (stl/char_traits.h `class char_traits`, stl/_alloc.h `class allocator`).
namespace std {
// STLport 4.6.2: char_traits / allocator / basic_string are `class` templates (mangled V); wint_t == unsigned short.
template <class _Tp> class allocator;
template <class _CharT, class _IntT> class __char_traits_base { public:
    typedef _CharT char_type;
    static bool __cdecl eq(const char_type& __c1, const char_type& __c2) { return __c1 == __c2; }
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60
    static unsigned int __cdecl length(const char_type* __s) {
        const _CharT _NullChar = _CharT();
        unsigned int __i;
        for (__i = 0; !eq(__s[__i], _NullChar); ++__i) {}
        return __i;
    }
};
template <class _CharT> class char_traits : public __char_traits_base<_CharT, _CharT> { };
template <class _CharT, class _Traits, class _Alloc> class basic_string { public:
    _CharT* _M_start;                     // _String_base<_CharT,_Alloc>
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const _CharT* data() const { return _M_start; }
};
}

namespace std {
template <class _CharT, class _Traits, class _Alloc>
inline bool __cdecl operator==(const basic_string<_CharT, _Traits, _Alloc>& __s, const _CharT* __t)
{
    unsigned int __n = _Traits::length(__t);
    return __s.size() == __n && _Traits::compare(__s.data(), __t, __n) == 0;
}
template bool __cdecl operator==(const basic_string<unsigned short, char_traits<unsigned short>, allocator<unsigned short> >&,
                                 const unsigned short*);
}
