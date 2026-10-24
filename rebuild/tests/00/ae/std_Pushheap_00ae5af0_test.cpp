#include <cstdio>
// Behaviour test for 0x00AE5AF0: _Push_heap over pair<long, unsigned long> with the lexicographic pair operator<.
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> inline bool operator<(const pair<K, V>& l, const pair<K, V>& r)
{
    return l.first < r.first || !(r.first < l.first) && l.second < r.second;
}
template<class T> struct less { bool operator()(const T& l, const T& r) const { return l < r; } };
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<long, unsigned long> Elem;
int main()
{
    Elem h[16]; int n = 0;
    long k[9] = { 3, 3, 7, 1, 7, 2, 0, 5, 3 };
    unsigned long s[9] = { 4, 9, 1, 8, 6, 2, 5, 5, 1 };
    for (int i = 0; i < 9; ++i) {
        Elem v; v.first = k[i]; v.second = s[i];
        _Push_heap(h, n, 0, v, less<Elem>()); ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2] < h[c]) { std::printf("00ae5af0_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 7 || h[0].second != 6) { std::printf("00ae5af0_TEST FAIL\n"); return 1; }
    std::printf("00ae5af0_TEST PASS\n");
    return 0;
}