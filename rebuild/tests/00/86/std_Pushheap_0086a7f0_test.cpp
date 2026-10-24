#include <cstdio>
// Behaviour test for 0x0086a7f0: _Push_heap over a 16-byte pair<long, CRandomAppearanceMorph::CSkeletalMorph> keyed max-heap.
struct CDefString { long TablePos; };
struct CSkeletalMorph { long GroupID; CDefString Name; bool AllowVariation; };
typedef CSkeletalMorph Payload;
#define SETPAY(p,i) ((p).GroupID = (i), (p).Name.TablePos = 10 * (i), (p).AllowVariation = ((i) & 1) != 0)
#define CHKPAY(p,i) ((p).GroupID == (i) && (p).Name.TablePos == 10 * (i) && (p).AllowVariation == (((i) & 1) != 0))
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> struct CKeyPairCompareLess {
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
typedef pair<long, Payload> Elem;
int main()
{
    Elem h[12]; int n = 0;
    long keys[9] = { 4, -9, 12, 2, 9, -1, 7, 3, 8 };
    for (int i = 0; i < 9; ++i) {
        Elem v; v.first = keys[i]; SETPAY(v.second, i);
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<long, Payload>()); ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("0086a7f0_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != keys[2] || !CHKPAY(h[0].second, 2)) { std::printf("0086a7f0_TEST FAIL\n"); return 1; }
    std::printf("0086a7f0_TEST PASS\n");
    return 0;
}