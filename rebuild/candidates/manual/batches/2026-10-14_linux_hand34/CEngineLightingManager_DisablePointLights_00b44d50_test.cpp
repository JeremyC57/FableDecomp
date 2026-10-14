#include <cstdio>
static int g_n = 0; static long g_idx[8];
struct CShaderRenderManager { void ResetLight(long i) { g_idx[g_n++] = i; } };
static CShaderRenderManager g_m; static CShaderRenderManager* g_pShaderRenderManager_01436e14 = &g_m;
struct CEngineLightingManager { unsigned char _pad[0xa0]; long m_shaderPointLightsEnabledCount;
    void DisablePointLights() { for (long i = 0; i < m_shaderPointLightsEnabledCount; ++i) g_pShaderRenderManager_01436e14->ResetLight(i + 1); m_shaderPointLightsEnabledCount = 0; } };
int main() {
    CEngineLightingManager m; m.m_shaderPointLightsEnabledCount = 3; m.DisablePointLights();
    if (g_n != 3 || g_idx[0] != 1 || g_idx[1] != 2 || g_idx[2] != 3 || m.m_shaderPointLightsEnabledCount != 0) { std::printf("00b44d50_TEST FAIL\n"); return 1; }
    m.DisablePointLights(); if (g_n != 3) { std::printf("00b44d50_TEST FAIL\n"); return 1; }
    std::printf("00b44d50_TEST PASS\n"); return 0;
}
