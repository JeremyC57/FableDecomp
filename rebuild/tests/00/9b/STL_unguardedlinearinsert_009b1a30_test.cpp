// Behaviour test for 0x009B1A30 __unguarded_linear_insert over 12-byte pair<unsigned, {int,int}>: the last entry
// slides left past every larger key (unsigned, keys only), carrying both value words.
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

typedef S::pair<unsigned int, SValue8> E;
static E mk(unsigned int k, int v) { SValue8 s; s.a = v; s.b = -v; return E(k, s); }
int main()
{
    E a[4] = { mk(0u, 1), mk(7u, 2), mk(0xF0000000u, 3), mk(6u, 4) };
    S::__unguarded_linear_insert(a + 3, E(a[3]), CKeyPairCompareLess<unsigned int, SValue8>());
    unsigned int ek[4] = { 0u, 6u, 7u, 0xF0000000u };
    int ev[4] = { 1, 4, 2, 3 };
    for (int i = 0; i < 4; ++i)
        if (a[i].first != ek[i] || a[i].second.a != ev[i] || a[i].second.b != -ev[i]) { std::printf("009b1a30_TEST FAIL\n"); return 1; }
    std::printf("009b1a30_TEST PASS\n");
    return 0;
}