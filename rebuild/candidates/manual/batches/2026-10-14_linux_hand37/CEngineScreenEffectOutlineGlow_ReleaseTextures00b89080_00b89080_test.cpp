#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_released = 0; static void* g_releasedPtr[4];
struct CTexturePool_00f36f00 { void Release(void* t) { g_releasedPtr[g_released++] = t; } };
struct CEngineGlobal_01436e98 { unsigned char _pad[0x3190]; CTexturePool_00f36f00 m_texturePool; }; static CEngineGlobal_01436e98 g_eng; static CEngineGlobal_01436e98* g_pEngine_01436e98 = &g_eng;
struct CEngineScreenEffectOutlineGlow { unsigned char _pad[0xc]; long m_heldA; long m_heldB; long m_heldC; void* m_pTextureA; void* m_pTextureB; void* m_pTextureC;
    void ReleaseTextures_00b89080() { CTexturePool_00f36f00* pool = &g_pEngine_01436e98->m_texturePool; if (m_heldA) { pool->Release(m_pTextureA); m_heldA = 0; } if (m_heldB) { pool->Release(m_pTextureB); m_heldB = 0; } if (m_heldC) { pool->Release(m_pTextureC); m_heldC = 0; } } };
int main() {
    CEngineScreenEffectOutlineGlow g; int a, b, c; g.m_heldA = 1; g.m_heldB = 0; g.m_heldC = 1; g.m_pTextureA = &a; g.m_pTextureB = &b; g.m_pTextureC = &c; g.ReleaseTextures_00b89080();
    if (g_released != 2 || g_releasedPtr[0] != &a || g_releasedPtr[1] != &c || g.m_heldA || g.m_heldC) { std::printf("00b89080_TEST FAIL\n"); return 1; }
    std::printf("00b89080_TEST PASS\n"); return 0;
}
