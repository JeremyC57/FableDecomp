// Behaviour test for std::__unguarded_insertion_sort_aux(pair<CCharString, long>*, ..., CKeyPairCompareLess<CCharString, long>) (0x009B5A20): after the pass [first, last) is in ascending key order (the
// entry before first is a sentinel no greater than any key), equal keys keep their order and payloads travel with keys.
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* Text;
    CCharString() : Text("") { }
    CCharString(const char* t) : Text(t) { }
    CCharString(const CCharString& o) : Text(o.Text) { }
    const CCharString& operator=(const CCharString& o) { Text = o.Text; return *this; }
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
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

typedef std::pair<CCharString, long> E;

int main()
{
    static const char* names[6] = { "delta", "alpha", "echo", "bravo", "alpha", "charlie" };
    E a[7];
    a[0] = E(CCharString(""), -1L);
    for (int i = 0; i < 6; ++i)
        a[i + 1] = E(CCharString(names[i]), (long)i);
    std::__unguarded_insertion_sort_aux(a + 1, a + 7, (E*)0, CKeyPairCompareLess<CCharString, long>());
    static const char* sorted[6] = { "alpha", "alpha", "bravo", "charlie", "delta", "echo" };
    static const int order[6] = { 1, 4, 3, 5, 0, 2 };
    int fails = 0;
    for (int i = 0; i < 6; ++i) {
        if (std::strcmp(a[i + 1].first.Text, sorted[i]) != 0) ++fails;
        if ((int)a[i + 1].second != order[i]) ++fails;
    }
    if (fails) { std::printf("009b5a20_TEST FAIL\n"); return 1; }
    std::printf("009b5a20_TEST PASS\n");
    return 0;
}