// behaviour test for 0x00AE71A0 std::sort_heap<pair<long,unsigned long>*,less<...> >, linked against the built
// function: a max-heap of (long, unsigned long) pairs comes out sorted ascending (signed first, unsigned second as the
// tie-break) with every pair intact; one- and two-element ranges
#include <cstdio>
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first; _T2 second;
    pair(_T1 a, _T2 b) : first(a), second(b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
typedef pair<long, unsigned long> P;
template void __fastcall __push_heap<P*, int, P, less<P> >(P*, int, int, P, less<P>);
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template void __fastcall __adjust_heap<P*, int, P, less<P> >(P*, int, int, P, less<P>);
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);
}
typedef std::pair<long, unsigned long> P;
typedef std::less<P> L;
static void MakeHeap(P* a, int n)
{
    for (int parent = (n - 2) / 2; ; --parent) {
        std::__adjust_heap<P*, int, P, L>(a, parent, n, P(a[parent]), L());
        if (parent == 0) break;
    }
}
static void SortHeap(P* a, int n) { std::sort_heap<P*, L>(a, a + n, L()); }
int main()
{
    static const long k[10] = { 5, -3, 12, 7, 7, 0, 99, -40, 2, 7 };
    static const unsigned long s[10] = { 4, 9, 1, 0x80000000u, 2, 8, 6, 5, 7, 3 };
    P a[10] = { P(k[0], s[0]), P(k[1], s[1]), P(k[2], s[2]), P(k[3], s[3]), P(k[4], s[4]),
                P(k[5], s[5]), P(k[6], s[6]), P(k[7], s[7]), P(k[8], s[8]), P(k[9], s[9]) };
    MakeHeap(a, 10);
    bool ok = a[0].first == 99 && a[0].second == 6;
    for (int i = 1; i < 10; ++i)
        if (a[(i - 1) / 2] < a[i]) ok = false;
    SortHeap(a, 10);
    for (int i = 0; i + 1 < 10; ++i)
        if (a[i + 1] < a[i]) ok = false;
    long sumk = 0; unsigned long sums = 0;
    for (int i = 0; i < 10; ++i) { sumk += a[i].first; sums += a[i].second; }
    ok = ok && sumk == 5 - 3 + 12 + 7 + 7 + 0 + 99 - 40 + 2 + 7 && sums == 4 + 9 + 1 + 0x80000000u + 2 + 8 + 6 + 5 + 7 + 3;
    ok = ok && a[0].first == -40 && a[5].first == 7 && a[5].second == 2 && a[6].second == 3 && a[7].second == 0x80000000u;
    P one[1] = { P(3, 1) };
    SortHeap(one, 1);
    ok = ok && one[0].first == 3 && one[0].second == 1;
    P two[2] = { P(4, 2), P(4, 1) };
    SortHeap(two, 2);
    ok = ok && two[0].second == 1 && two[1].second == 2;
    if (!ok) { std::printf("00ae71a0_TEST FAIL\n"); return 1; }
    std::printf("00ae71a0_TEST PASS\n");
    return 0;
}
