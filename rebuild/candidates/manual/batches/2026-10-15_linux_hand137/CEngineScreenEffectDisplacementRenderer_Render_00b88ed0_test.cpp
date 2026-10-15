// Behaviour test for 0x00B88ED0 CEngineScreenEffectDisplacementRenderer::Render: only the two passes; content -> copy/apply/show; textures released.
#include <cstdio>

static int g_log[16]; static int g_n = 0; static long g_released = -1; static int g_copy;
struct CParams { long a; unsigned int m_pass; };
struct CDisplay { int* CopyBackBufferToTexture() { g_log[g_n++] = 1; return &g_copy; } };
static CDisplay g_d; static CDisplay* g_pDisplay = &g_d;
struct CPool { void Release(long t) { g_released = t; g_log[g_n++] = 4; } };
struct CEngineGlobal { CPool m_texturePool; };
static CEngineGlobal g_e; static CEngineGlobal* g_pEngine = &g_e;
struct CRenderer {
    unsigned char m_enabled; int* BackBufferCopyTexture; int* DisplacementTexture; long DisplacementTextureHandle; bool HasContent;
    void ApplyDisplacement() { g_log[g_n++] = 2; }
    void ShowDisplacementTexture() { g_log[g_n++] = 3; }
    void ReleaseDynamicTextures()
    {
        CPool* pool = &g_pEngine->m_texturePool;
        if (BackBufferCopyTexture) BackBufferCopyTexture = 0;
        if (DisplacementTexture) { pool->Release(DisplacementTextureHandle); DisplacementTexture = 0; }
    }
    void Render(const CParams& params)
    {
        if (m_enabled) {
            unsigned int pass = params.m_pass;
            if (pass == 0x20000000 || pass == 0x10000) {
                if (HasContent) {
                    BackBufferCopyTexture = g_pDisplay->CopyBackBufferToTexture();
                    ApplyDisplacement(); ShowDisplacementTexture(); HasContent = false;
                }
                ReleaseDynamicTextures();
            }
        }
    }
};

int main()
{
    int tex;
    CRenderer r; r.m_enabled = 1; r.BackBufferCopyTexture = 0; r.DisplacementTexture = &tex; r.DisplacementTextureHandle = 5; r.HasContent = true;
    CParams other; other.m_pass = 0x100; r.Render(other);
    if (g_n != 0) { std::printf("00b88ed0_TEST FAIL\n"); return 1; }
    CParams p; p.m_pass = 0x10000; r.Render(p);
    if (g_n != 4 || g_log[0] != 1 || g_log[3] != 4 || r.HasContent || r.BackBufferCopyTexture || r.DisplacementTexture || g_released != 5) { std::printf("00b88ed0_TEST FAIL\n"); return 1; }
    r.m_enabled = 0; p.m_pass = 0x20000000; r.HasContent = true; r.Render(p);
    if (g_n != 4) { std::printf("00b88ed0_TEST FAIL\n"); return 1; }
    std::printf("00b88ed0_TEST PASS\n");
    return 0;
}
