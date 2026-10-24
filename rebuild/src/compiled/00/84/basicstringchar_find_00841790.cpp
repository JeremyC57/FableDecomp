// basic_string<char>::find(const char* s, unsigned int pos) const -- STLport string search: the index of the first occurrence of the C string s at or after pos, or npos
// Leaf: ?find@?$basic_string@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00841790, 84 bytes, __fastcall, `ret 8`.
// Identity: STLport 4.x `size_type find(const _CharT* __s, size_type __pos = 0) const { return find(__s, __pos,
//   _Traits::length(__s)); }` with the out-of-line find(s, pos, n) inlined: strlen loop, `pos + n > size()` -> npos,
//   else std::search over [start + pos, finish) for [s, s + n) and `result != finish ? result - start : npos`. Caller
//   LHHttp2 URL parsing (manifest "LHHttp2_ParseURL"); TLC has no PDB names for its STLport instantiations, so the
//   template's own name is used. The manifest label "StringSearch_FindPatternInRange" is an autoname.
// Callee: std::search<const char*, const char*, _Eq_traits<char_traits<char> > > 0x00567120 (__fastcall under /Gr: first1
//   in ecx, last1 in edx; first2, last2 and the empty predicate by value on the stack -- retail pushes the `s` argument
//   slot as the predicate's uninitialised dword). Its STLport body sits in the TU (`__declspec(noinline)`), so VC7.1
//   knows it writes no memory and keeps start / finish cached across the call (a bare declaration reloads both).
#include <string.h>
#pragma intrinsic(strlen)

namespace std {
template <class _CharT> struct char_traits {
    static size_t length(const _CharT* s) { return strlen(s); }
};
template <class _Tp> class allocator { };
template <class _Traits> struct _Eq_traits {
    _Eq_traits() { }                               // empty predicate: no store for the by-value temporary
    bool operator()(const char& a, const char& b) const { return a == b; }
};
template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
__declspec(noinline) _ForwardIter1 __fastcall search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                                                     _ForwardIter2 __first2, _ForwardIter2 __last2,
                                                     _BinaryPred __predicate)   // 0x00567120, STLport _algo.c body
{
    if (__first1 == __last1 || __first2 == __last2)
        return __first1;

    _ForwardIter2 __tmp(__first2);
    ++__tmp;
    if (__tmp == __last2) {
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        return __first1;
    }

    _ForwardIter2 __p1, __p;
    __p1 = __first2; ++__p1;

    while (__first1 != __last1) {
        while (__first1 != __last1) {
            if (__predicate(*__first1, *__first2))
                break;
            ++__first1;
        }
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        if (__first1 == __last1)
            return __last1;

        __p = __p1;
        _ForwardIter1 __current = __first1;
        if (++__current == __last1) return __last1;

        while (__predicate(*__current, *__p)) {
            if (++__p == __last2)
                return __first1;
            if (++__current == __last1)
                return __last1;
        }

        ++__first1;
    }
    return __first1;
}

template <class _CharT, class _Traits, class _Alloc> class basic_string {
public:
    typedef size_t size_type;
    typedef const _CharT* const_pointer;
    static const size_type npos = (size_type)-1;
    size_type size() const { return _M_finish - _M_start; }
    size_type find(const _CharT* __s, size_type __pos = 0) const { return find(__s, __pos, _Traits::length(__s)); }
    size_type find(const _CharT* __s, size_type __pos, size_type __n) const;
    _CharT* _M_start;
    _CharT* _M_finish;
    _CharT* _M_end_of_storage;
};

template <class _CharT, class _Traits, class _Alloc>
typename basic_string<_CharT, _Traits, _Alloc>::size_type
basic_string<_CharT, _Traits, _Alloc>::find(const _CharT* __s, size_type __pos, size_type __n) const   // STLport _string.c
{
    if (__pos + __n > size())
        return npos;
    else {
        const const_pointer __result =
            search((const _CharT*)this->_M_start + __pos, (const _CharT*)this->_M_finish,
                   __s, __s + __n, _Eq_traits<_Traits>());
        return __result != this->_M_finish ? __result - this->_M_start : npos;
    }
}

template basic_string<char, char_traits<char>, allocator<char> >::size_type
basic_string<char, char_traits<char>, allocator<char> >::find(const char*, size_type) const;
}