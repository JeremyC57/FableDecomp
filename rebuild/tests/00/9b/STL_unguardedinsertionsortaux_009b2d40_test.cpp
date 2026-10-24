// Behaviour test for 0x009B2D40 __unguarded_insertion_sort_aux over 8-byte pair<unsigned, int>: with a minimal sentinel before the
// range, every element is linearly inserted, so the range ends sorted by key and values travel with their keys.
#include <cstdio>
namespace S {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair<_T1, _T2>& o) : first(o.first), second(o.second) {}
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last; --__next;
    while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}
struct SValue8 { int a; int b; };
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const S::pair<K, V>& a, const S::pair<K, V>& b) const { return a.first < b.first; }
};

typedef S::pair<unsigned int, int> E;
static E mk(unsigned int k, int v) { return E(k, v); }
static int val(const E& e) { return (int)e.second; }
int main()
{
    unsigned int keys[8] = { 0u, 9u, 3u, 0xFFFFFFF0u, 3u, 1u, 8u, 2u };
    E a[8];
    for (int i = 0; i < 8; ++i) a[i] = mk(keys[i], 10 + i);
    // a[0] (key 0) is the sentinel; sort [1, 8)
    S::__unguarded_insertion_sort_aux(a + 1, a + 8, (E*)0, CKeyPairCompareLess<unsigned int, int>());
    unsigned int ek[8] = { 0u, 1u, 2u, 3u, 3u, 8u, 9u, 0xFFFFFFF0u };
    int ev[8] = { 10, 15, 17, 12, 14, 16, 11, 13 };
    for (int i = 0; i < 8; ++i)
        if (a[i].first != ek[i] || val(a[i]) != ev[i]) { std::printf("009b2d40_TEST FAIL\n"); return 1; }
    // empty range: no-op
    S::__unguarded_insertion_sort_aux(a + 3, a + 3, (E*)0, CKeyPairCompareLess<unsigned int, int>());
    if (a[3].first != 3u) { std::printf("009b2d40_TEST FAIL\n"); return 1; }
    std::printf("009b2d40_TEST PASS\n");
    return 0;
}