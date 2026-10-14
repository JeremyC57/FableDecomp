#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEngineScreenEffectDisplacementRenderer; static CEngineScreenEffectDisplacementRenderer* g_clearOnUninit = 0; static void ClearActive(CEngineScreenEffectDisplacementRenderer*);
static void* g_target = 0; static void* g_viewport = 0; static int g_uninit = 0; static void* g_released = 0;
struct CRenderTarget { unsigned char _body[0x24]; void Uninitialise() { ++g_uninit; if (g_clearOnUninit) ClearActive(g_clearOnUninit); } };
struct CDisplayManager { void SetRenderTarget(CRenderTarget* t) { g_target = t; } void SetViewport(void* v) { g_viewport = v; } }; static CDisplayManager g_dm; static CDisplayManager* g_pDisplayManager_01436e1c = &g_dm;
struct CTexturePool_00f36f00 { void Release(void* t) { g_released = t; } };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CTexturePool_00f36f00 m_texturePool; }; static CEngineGlobal_01436e98 g_eng; static CEngineGlobal_01436e98* g_pEngine_01436e98 = &g_eng;
struct CEngineScreenEffectDisplacementRenderer { unsigned char _pad[8]; bool m_active; unsigned char _pad09[7]; long m_textureState; void* m_pTexture; unsigned char _pad18[0xd8]; CRenderTarget m_renderTarget; unsigned char m_viewport[0x10];
    void FinishDisplacementRendering() { if (m_active) { g_pDisplayManager_01436e1c->SetRenderTarget(&m_renderTarget); g_pDisplayManager_01436e1c->SetViewport(m_viewport); m_renderTarget.Uninitialise(); if (!m_active) { g_pEngine_01436e98->m_texturePool.Release(m_pTexture); m_textureState = 0; } } } };
static void ClearActive(CEngineScreenEffectDisplacementRenderer* r) { r->m_active = false; }
int main() {
    CEngineScreenEffectDisplacementRenderer* r = (CEngineScreenEffectDisplacementRenderer*)new char[sizeof(CEngineScreenEffectDisplacementRenderer)]; int tex = 0; r->m_pTexture = &tex; r->m_textureState = 5;
    r->m_active = false; r->FinishDisplacementRendering(); if (g_target || g_uninit) { std::printf("00b869a0_TEST FAIL\n"); return 1; }
    r->m_active = true; g_clearOnUninit = r; r->FinishDisplacementRendering(); if (g_target != &r->m_renderTarget || g_viewport != r->m_viewport || g_uninit != 1 || g_released != &tex || r->m_textureState != 0) { std::printf("00b869a0_TEST FAIL\n"); return 1; }
    r->m_active = true; g_clearOnUninit = 0; r->m_textureState = 5; r->FinishDisplacementRendering(); if (g_uninit != 2 || r->m_textureState != 5) { std::printf("00b869a0_TEST FAIL\n"); return 1; }
    delete[] (char*)r;
    std::printf("00b869a0_TEST PASS\n"); return 0;
}
