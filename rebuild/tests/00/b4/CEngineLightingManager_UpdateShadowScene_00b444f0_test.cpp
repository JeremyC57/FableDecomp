#include <cstdio>
static int g_calls = 0; static long g_shadowed = -1; static void* g_params = 0; static void* g_scene = 0;
class CEngineShadowRenderer { public: void UpdateShadowScene(void* scene, long shadowed, void* params) { ++g_calls; g_shadowed = shadowed; g_params = params; g_scene = scene; } };
static CEngineShadowRenderer g_r; static CEngineShadowRenderer* g_pShadowRenderer_01436e60 = &g_r;
class CEngineLightingManager { public: unsigned char _pad[0x4694]; long m_shadowMode; unsigned char m_spotLightParams[0x38];
    void UpdateShadowScene(void* scene) { if (m_shadowMode == 2) g_pShadowRenderer_01436e60->UpdateShadowScene(scene, 1, m_spotLightParams); else if (m_shadowMode == 1) g_pShadowRenderer_01436e60->UpdateShadowScene(scene, 0, m_spotLightParams); } };
int main() {
    CEngineLightingManager* m = (CEngineLightingManager*)new char[sizeof(CEngineLightingManager)]; int scene = 0;
    m->m_shadowMode = 2; m->UpdateShadowScene(&scene); if (g_calls != 1 || g_shadowed != 1 || g_params != m->m_spotLightParams || g_scene != &scene) { std::printf("00b444f0_TEST FAIL\n"); return 1; }
    m->m_shadowMode = 1; m->UpdateShadowScene(&scene); if (g_calls != 2 || g_shadowed != 0) { std::printf("00b444f0_TEST FAIL\n"); return 1; }
    m->m_shadowMode = 0; m->UpdateShadowScene(&scene); if (g_calls != 2) { std::printf("00b444f0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00b444f0_TEST PASS\n"); return 0;
}