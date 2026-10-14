#include <cstdio>
#include <cstdlib>
#include <cstring>
static void* g_released = 0;
struct CTexturePool_00f36f00 { void Release(void* t) { g_released = t; } };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CTexturePool_00f36f00 m_texturePool; }; static CEngineGlobal_01436e98 g_eng; static CEngineGlobal_01436e98* g_pEngine_01436e98 = &g_eng;
struct CEngineScreenEffectDisplacementRenderer { unsigned char _pad[0xc]; void* m_pHandle; long m_textureState; void* m_pTexture;
    void ReleaseDynamicTextures_00b86a30() { CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool; if (m_pHandle) m_pHandle = 0; if (m_textureState) { pool->Release(m_pTexture); m_textureState = 0; } } };
int main() {
    CEngineScreenEffectDisplacementRenderer r; int h = 0, t = 0; r.m_pHandle = &h; r.m_textureState = 0; r.m_pTexture = &t; r.ReleaseDynamicTextures_00b86a30(); if (r.m_pHandle || g_released) { std::printf("00b86a30_TEST FAIL\n"); return 1; }
    r.m_textureState = 3; r.ReleaseDynamicTextures_00b86a30(); if (g_released != &t || r.m_textureState) { std::printf("00b86a30_TEST FAIL\n"); return 1; }
    std::printf("00b86a30_TEST PASS\n"); return 0;
}
