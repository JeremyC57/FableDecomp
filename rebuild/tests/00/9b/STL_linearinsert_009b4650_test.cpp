// Behaviour test for 0x009B4650 __linear_insert over 12-byte pair<unsigned, {int,int}>: a new minimum shifts the whole sorted prefix up one
// slot and lands at the front; any other key goes through the unguarded insert; values travel with keys (unsigned keys).
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
template <class _RI, class _BI>
_BI copy_backward(_RI __first, _RI __last, _BI __result)
{
    for (int __n = (int)(__last - __first); __n > 0; --__n) *--__result = *--__last;
    return __result;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) { copy_backward(__first, __last, __last + 1); *__first = __val; }
    else __unguarded_linear_insert(__last, __val, __comp);
}
}
struct SValue8 { int a; int b; };
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const S::pair<K, V>& a, const S::pair<K, V>& b) const { return a.first < b.first; }
};

typedef S::pair<unsigned int, SValue8> E;
static E mk(unsigned int k, int v) { SValue8 s; s.a = v; s.b = v * 3; return E(k, s); }
static int val(const E& e) { return e.second.b == e.second.a * 3 ? e.second.a : -1; }
static bool check(const E* a, const unsigned int* k, const int* v, int n)
{
    for (int i = 0; i < n; ++i) if (a[i].first != k[i] || val(a[i]) != v[i]) return false;
    return true;
}
int main()
{
    E a[5] = { mk(4u, 1), mk(6u, 2), mk(0x90000000u, 3), mk(8u, 4), mk(1u, 5) };
    // insert a[3] (key 8) into sorted [0,3): unguarded path
    S::__linear_insert(a, a + 3, E(a[3]), CKeyPairCompareLess<unsigned int, SValue8>());
    unsigned int k1[4] = { 4u, 6u, 8u, 0x90000000u }; int v1[4] = { 1, 2, 4, 3 };
    if (!check(a, k1, v1, 4)) { std::printf("009b4650_TEST FAIL\n"); return 1; }
    // insert a[4] (key 1) into sorted [0,4): new minimum, prefix shift
    S::__linear_insert(a, a + 4, E(a[4]), CKeyPairCompareLess<unsigned int, SValue8>());
    unsigned int k2[5] = { 1u, 4u, 6u, 8u, 0x90000000u }; int v2[5] = { 5, 1, 2, 4, 3 };
    if (!check(a, k2, v2, 5)) { std::printf("009b4650_TEST FAIL\n"); return 1; }
    // equal to the minimum: not less, goes unguarded and stays after the equal key
    E b[2] = { mk(3u, 7), mk(3u, 9) };
    S::__linear_insert(b, b + 1, E(b[1]), CKeyPairCompareLess<unsigned int, SValue8>());
    if (val(b[0]) != 7 || val(b[1]) != 9) { std::printf("009b4650_TEST FAIL\n"); return 1; }
    std::printf("009b4650_TEST PASS\n");
    return 0;
}