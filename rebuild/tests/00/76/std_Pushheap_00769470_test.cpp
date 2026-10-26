#include <cstdio>
// Behaviour test for 0x00769470: _Push_heap over a 16-byte pair<unsigned long, C3DVector> keyed max-heap.
class C3DVector { public: float X, Y, Z; };
typedef C3DVector Payload;
#define SETPAY(p,i) ((p).X = (float)(i), (p).Y = 2.0f * (i), (p).Z = -1.0f)
#define CHKPAY(p,i) ((p).X == (float)(i) && (p).Y == 2.0f * (i) && (p).Z == -1.0f)
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
    unsigned long keys[9] = { 4, 9, 0xFFFFFF00u, 2, 9, 1, 7, 3, 8 };
    for (int i = 0; i < 9; ++i) {
        Elem v; v.first = keys[i]; SETPAY(v.second, i);
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<unsigned long, Payload>()); ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00769470_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != keys[2] || !CHKPAY(h[0].second, 2)) { std::printf("00769470_TEST FAIL\n"); return 1; }
    std::printf("00769470_TEST PASS\n");
    return 0;
}