#include <cstdio>
// Behaviour test for 0x005F7DD4: Dinkumware _Push_heap over an 8-byte {signed key, dword} pair, max-heap on the key.
template<class K, class V> struct pair { K first; V second; };
struct CKeyPairCompareLess {
    bool operator()(const pair<long, long>& a, const pair<long, long>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
int main()
{
    pair<long, long> h[16];
    int n = 0;
    long keys[10] = { 5, -3, 9, 1, 9, 12, -7, 4, 0, 6 };
    for (int i = 0; i < 10; ++i) {
        pair<long, long> v; v.first = keys[i]; v.second = 100 + i;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess());
        ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("005f7dd4_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 12 || h[0].second != 105) { std::printf("005f7dd4_TEST FAIL\n"); return 1; }
    long sum = 0; for (int i = 0; i < n; ++i) sum += h[i].second;
    if (sum != 1045) { std::printf("005f7dd4_TEST FAIL\n"); return 1; }
    // top bound: with top == hole nothing moves
    pair<long, long> g[3]; g[0].first = 1; g[0].second = 0; g[1].first = 2; g[1].second = 0;
    pair<long, long> w; w.first = 50; w.second = 7;
    _Push_heap(g, 2, 2, w, CKeyPairCompareLess());
    if (g[2].first != 50 || g[0].first != 1) { std::printf("005f7dd4_TEST FAIL\n"); return 1; }
    std::printf("005f7dd4_TEST PASS\n");
    return 0;
}