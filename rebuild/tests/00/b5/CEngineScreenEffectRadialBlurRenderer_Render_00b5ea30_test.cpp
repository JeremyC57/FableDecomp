// Behaviour test for 0x00B5EA30 CEngineScreenEffectRadialBlurRenderer::Render: conditional capture/blur/debug, then release both textures.
#include <cstdio>

static int g_log[16]; static int g_n = 0; static long g_rel[4]; static int g_nr = 0;
struct CParams { int x; };
struct CPool { void Release(long t) { g_rel[g_nr++] = t; } };
struct CEngineGlobal { CPool m_texturePool; };
static CEngineGlobal g_e; static CEngineGlobal* g_pEngine = &g_e;
struct CRenderer {
    unsigned char m_enabled; int* BlurTexture256_1; int* BlurTexture256_2; long BlurTextureHandle256_1, BlurTextureHandle256_2;
    float CurrentParameters_Fade; long ShowDebug; bool UseEnvironmentParameters; bool m_tracked;
    void CaptureEnvironmentParameters() { g_log[g_n++] = 1; }
    bool UpdateWorldPositionTracking() { g_log[g_n++] = 2; return m_tracked; }
    void CaptureBackBuffer() { g_log[g_n++] = 3; }
    void ApplyRadialBlur() { g_log[g_n++] = 4; }
    void ShowBlurTexture() { g_log[g_n++] = 5; }
    void Render(const CParams&)
    {
        if (m_enabled) {
            if (UseEnvironmentParameters) CaptureEnvironmentParameters();
            if (UpdateWorldPositionTracking() && CurrentParameters_Fade > 1.0f / 255.0f) { CaptureBackBuffer(); ApplyRadialBlur(); }
            if (ShowDebug) ShowBlurTexture();
            if (BlurTexture256_1) {
                CPool* pool = &g_pEngine->m_texturePool;
                pool->Release(BlurTextureHandle256_1); pool->Release(BlurTextureHandle256_2);
                BlurTexture256_1 = 0; BlurTexture256_2 = 0;
            }
        }
    }
};

int main()
{
    int a, b; CParams p;
    CRenderer r; r.m_enabled = 1; r.BlurTexture256_1 = &a; r.BlurTexture256_2 = &b; r.BlurTextureHandle256_1 = 11; r.BlurTextureHandle256_2 = 12;
    r.CurrentParameters_Fade = 0.5f; r.ShowDebug = 1; r.UseEnvironmentParameters = true; r.m_tracked = true;
    r.Render(p);
    if (g_n != 5 || g_log[0] != 1 || g_log[2] != 3 || g_log[4] != 5 || g_nr != 2 || g_rel[0] != 11 || g_rel[1] != 12 || r.BlurTexture256_1 || r.BlurTexture256_2) { std::printf("00b5ea30_TEST FAIL\n"); return 1; }
    r.CurrentParameters_Fade = 0.001f; r.ShowDebug = 0; r.UseEnvironmentParameters = false;
    r.Render(p);
    if (g_n != 6 || g_log[5] != 2 || g_nr != 2) { std::printf("00b5ea30_TEST FAIL\n"); return 1; }
    std::printf("00b5ea30_TEST PASS\n");
    return 0;
}