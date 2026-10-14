#include <cstdio>

// Model of CCombatRenderer::SetRenderScaleWithFallback @ 0x00C3F710
static long g_tried[8]; static int g_nTried = 0; static long g_okScale = -1;
struct CRenderScaleConfig { unsigned char _pad[4]; long m_firstScale; };
struct CCombatRenderer {
    unsigned char _pad[0x188]; CRenderScaleConfig* m_pScaleConfig;
    bool SetRenderScale(long s) { g_tried[g_nTried++] = s; return s == g_okScale; }
    long GetFallbackRenderScale(long s) { return s / 2; }
    bool SetRenderScaleWithFallback() {
        CRenderScaleConfig* config = m_pScaleConfig;
        if (config != 0) { long scale = config->m_firstScale; while (scale != 0) { if (SetRenderScale(scale)) return true; scale = GetFallbackRenderScale(scale); } return false; }
        return SetRenderScale(4);
    }
};
int main()
{
    CCombatRenderer r; r.m_pScaleConfig = 0; g_okScale = 4;
    if (!r.SetRenderScaleWithFallback() || g_nTried != 1 || g_tried[0] != 4) { std::printf("00c3f710_TEST FAIL\n"); return 1; }
    CRenderScaleConfig cfg; cfg.m_firstScale = 8; r.m_pScaleConfig = &cfg; g_nTried = 0; g_okScale = 2;
    if (!r.SetRenderScaleWithFallback() || g_nTried != 3 || g_tried[0] != 8 || g_tried[1] != 4 || g_tried[2] != 2) { std::printf("00c3f710_TEST FAIL\n"); return 1; }
    g_nTried = 0; g_okScale = 3;
    if (r.SetRenderScaleWithFallback() || g_nTried != 4) { std::printf("00c3f710_TEST FAIL\n"); return 1; }
    std::printf("00c3f710_TEST PASS\n"); return 0;
}