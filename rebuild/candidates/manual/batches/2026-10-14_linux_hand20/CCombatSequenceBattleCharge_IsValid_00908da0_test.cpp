#include <cstdio>
struct CWorld_Retail { long frame; long GetCurrentFrame() { return frame; } };
static CWorld_Retail g_world; static CWorld_Retail* g_pWorld = &g_world; static long g_logicFramesPerSecond = 15;
struct CSeq { long m_frameOfLastUse; bool IsValid() { return (float)(g_pWorld->GetCurrentFrame() - m_frameOfLastUse) / g_logicFramesPerSecond > 10.0f; } };
int main()
{
    CSeq s; s.m_frameOfLastUse = 100;
    g_world.frame = 250; if (s.IsValid()) { std::printf("00908da0_TEST FAIL\n"); return 1; }
    g_world.frame = 251; if (!s.IsValid()) { std::printf("00908da0_TEST FAIL\n"); return 1; }
    g_world.frame = 50; if (s.IsValid()) { std::printf("00908da0_TEST FAIL\n"); return 1; }
    std::printf("00908da0_TEST PASS\n");
    return 0;
}
