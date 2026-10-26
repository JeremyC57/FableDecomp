#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_released = 0; static void* g_releasedPtr[4];
struct CTexturePool_00f36f00 { void Release(void* t) { g_releasedPtr[g_released++] = t; } };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CTexturePool_00f36f00 m_texturePool; }; static CEngineGlobal_01436e98 g_eng; static CEngineGlobal_01436e98* g_pEngine_01436e98 = &g_eng;
class CEngineScreenEffectRadialBlurRenderer { public: unsigned char _pad[0xc]; long m_held; long m_heldB; void* m_pTextureA; void* m_pTextureB;
    void ReleaseTextures_00b5bd30() { if (m_held) { CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool; pool->Release(m_pTextureA); pool->Release(m_pTextureB); m_held = 0; m_heldB = 0; } } };
int main() {
    CEngineScreenEffectRadialBlurRenderer r; int a, b; r.m_held = 0; r.m_heldB = 2; r.m_pTextureA = &a; r.m_pTextureB = &b; r.ReleaseTextures_00b5bd30(); if (g_released || r.m_heldB != 2) { std::printf("00b5bd30_TEST FAIL\n"); return 1; }
    r.m_held = 1; r.ReleaseTextures_00b5bd30(); if (g_released != 2 || g_releasedPtr[0] != &a || g_releasedPtr[1] != &b || r.m_held || r.m_heldB) { std::printf("00b5bd30_TEST FAIL\n"); return 1; }
    std::printf("00b5bd30_TEST PASS\n"); return 0;
}