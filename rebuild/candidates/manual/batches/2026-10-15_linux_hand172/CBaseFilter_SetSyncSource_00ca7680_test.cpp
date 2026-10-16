#include <cstdio>
static int g_locks = 0;
struct CClock { long refs; void AddRef() { ++refs; } void Release() { --refs; } };
struct CLock { CLock() { ++g_locks; } ~CLock() { --g_locks; } };
struct CFilter {
    CClock* m_pClock;
    long SetSyncSource(CClock* pClock) { CLock l; if (pClock) pClock->AddRef(); if (m_pClock) m_pClock->Release(); m_pClock = pClock; return 0; }
};
int main()
{
    CClock a = { 1 }, b = { 1 }; CFilter f = { 0 };
    f.SetSyncSource(&a); f.SetSyncSource(&b); f.SetSyncSource(&b);
    if (f.m_pClock != &b || a.refs != 1 || b.refs != 2 || g_locks != 0) { std::printf("00ca7680_TEST FAIL\n"); return 1; }
    f.SetSyncSource(0);
    if (f.m_pClock || b.refs != 1) { std::printf("00ca7680_TEST FAIL\n"); return 1; }
    std::printf("00ca7680_TEST PASS\n");
    return 0;
}
