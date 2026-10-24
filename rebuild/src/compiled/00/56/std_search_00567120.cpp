// std::search(const char* first1, const char* last1, const char* first2, const char* last2, _Eq_traits pred) -- STLport predicate search: the first position in [first1,last1) where [first2,last2) occurs
// Leaf: ??$search
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567120, 160 bytes, /Gr `__fastcall` (first1 in ecx, last1 in edx, first2 /
//   last2 / the empty predicate on the stack: `ret 0Ch`). Six callers, all STLport `basic_string<char>::find(
//   const char*, size_type, size_type)` bodies (0x0056096A, 0x00567A0C, 0x00568060, 0x0083F75D, ...), which
//   call `search(start + pos, finish, s, s + n, _Eq_traits<char_traits<char> >())`. The catalog label
//   "StringSearch_FindPattern" is descriptive.
// Callees: none.
// Shape: STLport 4.x `search(.., _BinaryPred)` as written: empty-range test, the one-character pattern loop,
//   then the general loop with its two consecutive scans for *first2 (the second one re-tests the match the
//   first already found: retail's second `cmp byte ptr [eax],cl` loop), `__p = __p1` with __p1 spilled to the
//   `push ecx` word and reloaded after a partial match. The predicate never reaches memory (its stack word
//   is never read).
namespace std {

struct char_traits_char {
    static bool eq(const char& __c1, const char& __c2) { return __c1 == __c2; }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function { };

template <class _Traits> struct _Eq_traits : public binary_function<char, char, bool> {
    bool operator()(const char& __x, const char& __y) const { return _Traits::eq(__x, __y); }
};

template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 __fastcall search(_ForwardIter1 __first1, _ForwardIter1 __last1,
                                _ForwardIter2 __first2, _ForwardIter2 __last2,
                                _BinaryPred __predicate)
{
    // Test for empty ranges
    if (__first1 == __last1 || __first2 == __last2)
        return __first1;

    // Test for a pattern of length 1.
    _ForwardIter2 __tmp(__first2);
    ++__tmp;
    if (__tmp == __last2) {
        while (__first1 != __last1 && !__predicate(*__first1, *__first2))
            ++__first1;
        return __first1;
    }

    // General case.
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
        if (++__current == __last1)
            return __last1;

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

template const char* __fastcall search<const char*, const char*, _Eq_traits<char_traits_char> >(
    const char*, const char*, const char*, const char*, _Eq_traits<char_traits_char>);

}