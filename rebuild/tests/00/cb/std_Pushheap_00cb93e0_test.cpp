#include <cstdio>
// Behaviour test for 0x00CB93E0: _Push_heap over pair<unsigned __int64, CPersistInfo{UID, CCharString}>.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_dtors = 0;
struct CCharString {
    int Data;
    CCharString() : Data(0) { }
    CCharString(const CCharString& o) : Data(o.Data) { }
    CCharString& operator=(const CCharString& o) { Data = o.Data; return *this; }
    ~CCharString() { ++g_dtors; }
};
struct CPersistInfo { u64 UID; CCharString PersistScript; };
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
typedef pair<u64, CPersistInfo> Elem;
int main()
{
    Elem h[10]; int n = 0;
    u64 keys[8] = { 4, ((u64)2 << 32), 0xFFFFFFFFu, 7, ((u64)2 << 32) + 1, 0, 3, ((u64)1 << 32) };
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = keys[i]; v.second.UID = keys[i] * 3 + 1; v.second.PersistScript.Data = 40 + i;
        int d0 = g_dtors;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<u64, CPersistInfo>()); ++n;
        if (g_dtors <= d0) { std::printf("00cb93e0_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00cb93e0_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != ((u64)2 << 32) + 1 || h[0].second.UID != (((u64)2 << 32) + 1) * 3 + 1 || h[0].second.PersistScript.Data != 44) {
        std::printf("00cb93e0_TEST FAIL\n"); return 1;
    }
    std::printf("00cb93e0_TEST PASS\n");
    return 0;
}