#include <cstdio>
struct CSim { virtual void v0(){} virtual void v1(){} virtual void v2(){} virtual void v3(){} virtual void v4(){} virtual void v5(){} virtual void v6(){} virtual bool IsRunning() { return running; } bool running; };
static CSim g_sim; static long g_lastIndex = -1;
struct CInfo { CSim* FindSimulation(long index) { g_lastIndex = index; return index == 3 ? &g_sim : 0; } };
struct CMesh { unsigned char _pad[0x64]; CInfo* m_pSimulationInfo;
    bool IsSimulationRunning(long index) { if (m_pSimulationInfo) { CSim* s = m_pSimulationInfo->FindSimulation(index); if (s && s->IsRunning()) return true; } return false; } };
int main()
{
    CInfo info; CMesh m; m.m_pSimulationInfo = 0;
    if (m.IsSimulationRunning(3)) { std::printf("00bebbb0_TEST FAIL\n"); return 1; }
    m.m_pSimulationInfo = &info; g_sim.running = false;
    if (m.IsSimulationRunning(3) || g_lastIndex != 3 || m.IsSimulationRunning(4)) { std::printf("00bebbb0_TEST FAIL\n"); return 1; }
    g_sim.running = true;
    if (!m.IsSimulationRunning(3)) { std::printf("00bebbb0_TEST FAIL\n"); return 1; }
    std::printf("00bebbb0_TEST PASS\n"); return 0;
}
