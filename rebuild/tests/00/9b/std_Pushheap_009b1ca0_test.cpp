#include <cstdio>
// Behaviour test for 0x009b1ca0: _Push_heap over a 12-byte pair<unsigned long, CTextureMorph> keyed max-heap.

struct Payload { long a; long b; };   // CTextureMorph: two dwords
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
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
typedef pair<unsigned long, Payload> Elem;
int main()
{
    Elem h[12]; int n = 0;
    int keys[9] = { 6, 2, 8, 8, 1, 11, 3, 0, 5 };
    for (int i = 0; i < 9; ++i) {
        Elem v; v.first = (unsigned long)keys[i]; v.second.a = 10 * i; v.second.b = -i;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<unsigned long, Payload>()); ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("009b1ca0_TEST FAIL\n"); return 1; }
    }
    if ((int)h[0].first != 11 || h[0].second.a != 50 || h[0].second.b != -5) { std::printf("009b1ca0_TEST FAIL\n"); return 1; }
    // a hole at the top bound stays put
    Elem g[2]; g[0].first = (unsigned long)1; Elem w; w.first = (unsigned long)9; w.second.a = 1; w.second.b = 2;
    _Push_heap(g, 1, 1, w, CKeyPairCompareLess<unsigned long, Payload>());
    if ((int)g[1].first != 9 || (int)g[0].first != 1) { std::printf("009b1ca0_TEST FAIL\n"); return 1; }
    std::printf("009b1ca0_TEST PASS\n");
    return 0;
}