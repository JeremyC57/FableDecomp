#include <cstdio>
struct CEngineCamera { int x; };
static int g_calls = 0; static float g_sum = 0; static unsigned long g_c = 0; static const CEngineCamera* g_cam = 0;
struct CGroup { unsigned char _pad[0xc]; CGroup* m_pNext; void RenderUpdate(CEngineCamera* cam, float a, float b, unsigned long c, float d) { ++g_calls; g_cam = cam; g_sum += a + b + d; g_c = c; } };
struct COwner { unsigned char _pad[0x20]; CGroup* m_pGroups; unsigned char _pad24[0x12]; unsigned char m_renderEnabled : 1;
    void RenderUpdateGroups(CEngineCamera* cam, float a, float b, unsigned long c, float d) { if (m_renderEnabled) { for (CGroup* g = m_pGroups; g; g = g->m_pNext) g->RenderUpdate(cam, a, b, c, d); } } };
int main()
{
    CEngineCamera cam; CGroup g2; g2.m_pNext = 0; CGroup g1; g1.m_pNext = &g2; COwner o; o.m_pGroups = &g1; o.m_renderEnabled = 0;
    o.RenderUpdateGroups(&cam, 1, 2, 3, 4); if (g_calls != 0) { std::printf("00bdd930_TEST FAIL\n"); return 1; }
    o.m_renderEnabled = 1; o.RenderUpdateGroups(&cam, 1.0f, 2.0f, 3, 4.0f);
    if (g_calls != 2 || g_sum != 14.0f || g_c != 3 || g_cam != &cam) { std::printf("00bdd930_TEST FAIL\n"); return 1; }
    o.m_pGroups = 0; o.RenderUpdateGroups(&cam, 1, 2, 3, 4); if (g_calls != 2) { std::printf("00bdd930_TEST FAIL\n"); return 1; }
    std::printf("00bdd930_TEST PASS\n"); return 0;
}
