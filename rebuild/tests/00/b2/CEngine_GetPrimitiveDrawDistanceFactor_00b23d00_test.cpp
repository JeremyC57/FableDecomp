#include <cstdio>
struct SRenderSettings { unsigned char _pad[0x50]; float m_distanceBias; }; struct SViewRecord { unsigned char _pad[0x7c]; float m_drawDistanceScale; };
static SRenderSettings g_rs; static SViewRecord g_vr; static SRenderSettings* g_pRenderSettings = &g_rs; static SViewRecord* g_pViewRecord = &g_vr;
struct CEngine { static float GetPrimitiveDrawDistanceFactor() { return (g_pRenderSettings->m_distanceBias + 1.0f) * g_pViewRecord->m_drawDistanceScale; } };
int main() { g_rs.m_distanceBias = 0.5f; g_vr.m_drawDistanceScale = 4.0f; if (CEngine::GetPrimitiveDrawDistanceFactor() != 6.0f) { std::printf("00b23d00_TEST FAIL\n"); return 1; } std::printf("00b23d00_TEST PASS\n"); return 0; }