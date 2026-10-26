// Behaviour test for 0x009B1B00 __unguarded_linear_insert over 8-byte pair<unsigned, int>: the last element
// slides left past every larger key (unsigned compare, keys only), carrying its value; the sentinel at [0] stops it.
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
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const S::pair<K, V>& a, const S::pair<K, V>& b) const { return a.first < b.first; }
};

typedef S::pair<unsigned int, int> E;
int main()
{
    E a[5];
    unsigned int k[5] = { 1u, 5u, 9u, 0x80000000u, 3u };
    for (int i = 0; i < 5; ++i) { a[i].first = k[i]; a[i].second = (int)(100 + i); }
    S::__unguarded_linear_insert(a + 4, E(a[4]), CKeyPairCompareLess<unsigned int, int>());
    unsigned int ek[5] = { 1u, 3u, 5u, 9u, 0x80000000u };
    int ev[5] = { 100, 104, 101, 102, 103 };
    for (int i = 0; i < 5; ++i)
        if (a[i].first != ek[i] || (int)a[i].second != ev[i]) { std::printf("009b1b00_TEST FAIL\n"); return 1; }
    // equal key: stops (strict less), stays put
    E b[3]; b[0] = E(2u, (int)7); b[1] = E(4u, (int)8); b[2] = E(4u, (int)9);
    S::__unguarded_linear_insert(b + 2, E(b[2]), CKeyPairCompareLess<unsigned int, int>());
    if (b[1].second != (int)8 || b[2].second != (int)9) { std::printf("009b1b00_TEST FAIL\n"); return 1; }
    std::printf("009b1b00_TEST PASS\n");
    return 0;
}