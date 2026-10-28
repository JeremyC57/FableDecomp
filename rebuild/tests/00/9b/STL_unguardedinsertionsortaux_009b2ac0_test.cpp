// Behaviour test for std::__unguarded_insertion_sort_aux(CWideString*, CWideString*, CWideString*, less<CWideString>) (0x009B2AC0): after the pass [first, last) is in ascending key order (the entry
// before first is a sentinel no greater than any key), equal keys keep their order and payloads travel with keys.
#include <cstdio>
#include <cstring>
#include <cstdlib>

class CWideString {
public:
    const unsigned short* Text;
    CWideString() : Text(0) { }
    explicit CWideString(const unsigned short* t) : Text(t) { }
    CWideString(const CWideString& o) : Text(o.Text) { }
    const CWideString& operator=(const CWideString& o) { Text = o.Text; return *this; }
    bool operator<(const CWideString& o) const
    {
        int i = 0;
        while (Text[i] != 0 && Text[i] == o.Text[i])
            ++i;
        return Text[i] < o.Text[i];
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

int main()
{
    static unsigned short w0[] = { 'd', 'u', 'k', 'e', 0 }, w1[] = { 'a', 'b', 0 }, w2[] = { 'e', 0 },
                          w3[] = { 'b', 'o', 'b', 0 }, w4[] = { 'a', 'b', 0 }, w5[] = { 'a', 'b', 'c', 0 }, w6[] = { 0 };
    static const unsigned short* words[6] = { w0, w1, w2, w3, w4, w5 };
    CWideString a[7];
    a[0] = CWideString(w6);
    for (int i = 0; i < 6; ++i)
        a[i + 1] = CWideString(words[i]);
    std::__unguarded_insertion_sort_aux(a + 1, a + 7, (CWideString*)0, std::less<CWideString>());
    static const int order[6] = { 1, 4, 5, 3, 0, 2 };   // ab ab abc bob duke e
    int fails = 0;
    for (int i = 0; i < 6; ++i)
        if (a[i + 1].Text != words[order[i]]) ++fails;
    if (fails) { std::printf("009b2ac0_TEST FAIL\n"); return 1; }
    std::printf("009b2ac0_TEST PASS\n");
    return 0;
}
