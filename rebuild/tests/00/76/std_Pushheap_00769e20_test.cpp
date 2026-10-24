#include <cstdio>
// Behaviour test for 0x00769E20: _Push_heap over pair<unsigned long, CParticleUpdateParamDesc> (polymorphic
// payload; the base destructor runs once per by-value argument).
static int g_baseDtors = 0;
struct CBase {
    virtual ~CBase() { ++g_baseDtors; }
};
struct CParticleUpdateParamCallback { int id; };
struct CParticleUpdateParamDesc : CBase {
    CParticleUpdateParamCallback* Callback;
    unsigned long Index;
};
struct CParticleUpdateCCompareParamDesc {
    template<class P> bool operator()(const P& a, const P& b) const { return a.first < b.first; }
};
template<class K, class V> struct pair { K first; V second; };
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<unsigned long, CParticleUpdateParamDesc> Elem;
int main()
{
    CParticleUpdateParamCallback cb[8];
    Elem h[8];
    unsigned long keys[7] = { 4, 10, 2, 7, 0xFFFFFFF0u, 3, 9 };
    int n = 0;
    for (int i = 0; i < 7; ++i) {
        Elem v; v.first = keys[i]; v.second.Callback = &cb[i]; v.second.Index = 100 + i;
        int before = g_baseDtors;
        _Push_heap(h, n, 0, v, CParticleUpdateCCompareParamDesc());
        if (g_baseDtors <= before) { std::printf("00769e20_TEST FAIL\n"); return 1; }
        ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00769e20_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != 0xFFFFFFF0u || h[0].second.Index != 104 || h[0].second.Callback != &cb[4]) {
        std::printf("00769e20_TEST FAIL\n"); return 1;
    }
    std::printf("00769e20_TEST PASS\n");
    return 0;
}