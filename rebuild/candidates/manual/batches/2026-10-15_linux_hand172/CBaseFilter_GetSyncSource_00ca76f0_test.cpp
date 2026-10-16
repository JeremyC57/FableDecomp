#include <cstdio>
struct CClock { long refs; void AddRef() { ++refs; } };
struct CFilter {
    CClock* m_pClock;
    long GetSyncSource(CClock** p) { if (p == 0) return (long)0x80004003; if (m_pClock) m_pClock->AddRef(); *p = m_pClock; return 0; }
};
int main()
{
    CClock a = { 1 }; CFilter f = { &a }, g = { 0 }; CClock* out = 0;
    if (f.GetSyncSource(0) != (long)0x80004003 || f.GetSyncSource(&out) != 0 || out != &a || a.refs != 2) { std::printf("00ca76f0_TEST FAIL\n"); return 1; }
    out = &a; if (g.GetSyncSource(&out) != 0 || out != 0) { std::printf("00ca76f0_TEST FAIL\n"); return 1; }
    std::printf("00ca76f0_TEST PASS\n");
    return 0;
}
