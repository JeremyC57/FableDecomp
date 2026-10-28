#include <cstdio>
// Behaviour test for 0x006BE9D0: _Push_heap over pair<long, CMusicManager::CRegisteredMusicEntry>.
static int g_strDtors = 0;
class CWideString { public:
    int Data;
    CWideString() : Data(0) { }
    CWideString(const CWideString& o) : Data(o.Data) { }
    const CWideString& operator=(const CWideString& o) { Data = o.Data; return *this; }
    ~CWideString() { ++g_strDtors; }
};
struct CRegisteredMusicEntry {
    CWideString SrcFileName;
    CWideString DstFileName;
    float Gain;
    float Offset;
    bool CacheToHD;
};
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
typedef pair<long, CRegisteredMusicEntry> Elem;
int main()
{
    Elem h[10]; int n = 0;
    long keys[8] = { 4, 1, 15, -3, 8, 15, 2, 20 };
    for (int i = 0; i < 8; ++i) {
        Elem v; v.first = keys[i];
        v.second.SrcFileName.Data = i; v.second.DstFileName.Data = 10 + i;
        v.second.Gain = 0.5f * i; v.second.Offset = -1.0f * i; v.second.CacheToHD = (i & 1) != 0;
        int d0 = g_strDtors;
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<long, CRegisteredMusicEntry>()); ++n;
        if (g_strDtors < d0 + 2) { std::printf("006be9d0_TEST FAIL\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("006be9d0_TEST FAIL\n"); return 1; }
    }
    const CRegisteredMusicEntry& e = h[0].second;
    if (h[0].first != 20 || e.SrcFileName.Data != 7 || e.DstFileName.Data != 17 || e.Gain != 3.5f || e.Offset != -7.0f || !e.CacheToHD) {
        std::printf("006be9d0_TEST FAIL\n"); return 1;
    }
    std::printf("006be9d0_TEST PASS\n");
    return 0;
}