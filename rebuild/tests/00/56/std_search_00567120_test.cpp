// Behaviour test for 0x00567120 std::search(.., _Eq_traits) over const char ranges.
#include <cstdio>
#include <cstring>

namespace stl {
struct char_traits_char { static bool eq(const char& __c1, const char& __c2) { return __c1 == __c2; } };
template <class _Traits> struct _Eq_traits {
    bool operator()(const char& __x, const char& __y) const { return _Traits::eq(__x, __y); }
};
template <class _ForwardIter1, class _ForwardIter2, class _BinaryPred>
_ForwardIter1 search(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2, _ForwardIter2 __last2,
                     _BinaryPred __predicate)
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
}

static int Find(const char* hay, const char* needle)
{
    const char* l1 = hay + std::strlen(hay);
    const char* r = stl::search(hay, l1, needle, needle + std::strlen(needle), stl::_Eq_traits<stl::char_traits_char>());
    return (int)(r - hay);
}

int main()
{
    int ok = 1;
    if (Find("hello world", "world") != 6) ok = 0;
    if (Find("hello world", "o") != 4) ok = 0;          // one-character pattern
    if (Find("hello world", "xyz") != 11) ok = 0;       // not found -> last1
    if (Find("hello world", "") != 0) ok = 0;           // empty pattern -> first1
    if (Find("", "abc") != 0) ok = 0;                   // empty haystack
    if (Find("aaab", "aab") != 1) ok = 0;               // partial match then restart
    if (Find("abcab", "abd") != 5) ok = 0;              // runs off the end mid-match
    if (Find("xxab", "ab") != 2) ok = 0;
    if (Find("ab", "abc") != 2) ok = 0;
    if (!ok) { std::printf("00567120_TEST FAIL\n"); return 1; }
    std::printf("00567120_TEST PASS\n");
    return 0;
}