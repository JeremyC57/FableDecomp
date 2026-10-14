#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_base = 0;
struct SPhysicsSettings_01436e68 { unsigned char _pad[0x1c]; unsigned char m_simulationCounter; unsigned char NextSimulationId() { unsigned char id = m_simulationCounter; m_simulationCounter = id + 1; return id; } };
static SPhysicsSettings_01436e68 g_settings; static SPhysicsSettings_01436e68* g_pPhysicsSettings_01436e68 = &g_settings;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct CEngineSimulationBase : CBase { long m_references; unsigned char m_id; CEngineSimulationBase() : CBase(), m_references(1), m_id(g_pPhysicsSettings_01436e68->NextSimulationId()) { } };
int main() {
    g_settings.m_simulationCounter = 254; CEngineSimulationBase a; CEngineSimulationBase b; CEngineSimulationBase c;
    if (a.m_id != 254 || b.m_id != 255 || c.m_id != 0 || g_settings.m_simulationCounter != 1 || a.m_references != 1 || g_base != 3) { std::printf("00b80900_TEST FAIL\n"); return 1; }
    std::printf("00b80900_TEST PASS\n"); return 0;
}