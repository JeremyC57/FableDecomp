// Behaviour test for 0x00B5EE00 CEngineScreenEffectGlowRenderer::SetEffectQuality: enable on success, free + disable otherwise; false only on init failure.
#include <cstdio>

static int g_freed = 0; static long g_freedPage = 0;
struct CPool { void FreeMemoryPage(long p) { ++g_freed; g_freedPage = p; } };
struct CEngineGlobal { CPool m_scratchTextures; };
static CEngineGlobal g_eng; static CEngineGlobal* g_pEngine = &g_eng;
struct CSettings { bool Enabled; };
struct CRenderer {
    unsigned char m_enabled; long DynamicTexturePageHandle; bool EffectSupported; bool m_initOk;
    bool InitialiseTextures() { return m_initOk; }
    void FreeTexturePage() { if (DynamicTexturePageHandle >= 0) g_pEngine->m_scratchTextures.FreeMemoryPage(DynamicTexturePageHandle); }
    bool SetEffectQuality(const CSettings& q)
    {
        if (q.Enabled && EffectSupported) {
            if (InitialiseTextures()) { m_enabled = true; return true; }
            FreeTexturePage(); m_enabled = false; return false;
        }
        m_enabled = false; FreeTexturePage();
        return true;
    }
};

int main()
{
    CSettings on; on.Enabled = true; CSettings off; off.Enabled = false;
    CRenderer r; r.m_enabled = 0; r.DynamicTexturePageHandle = 9; r.EffectSupported = true; r.m_initOk = true;
    if (!r.SetEffectQuality(on) || r.m_enabled != 1 || g_freed != 0) { std::printf("00b5ee00_TEST FAIL\n"); return 1; }
    r.m_initOk = false;
    if (r.SetEffectQuality(on) || r.m_enabled != 0 || g_freed != 1 || g_freedPage != 9) { std::printf("00b5ee00_TEST FAIL\n"); return 1; }
    r.m_enabled = 1;
    if (!r.SetEffectQuality(off) || r.m_enabled != 0 || g_freed != 2) { std::printf("00b5ee00_TEST FAIL\n"); return 1; }
    r.DynamicTexturePageHandle = -1; r.EffectSupported = false; r.m_initOk = true;
    if (!r.SetEffectQuality(on) || r.m_enabled != 0 || g_freed != 2) { std::printf("00b5ee00_TEST FAIL\n"); return 1; }
    std::printf("00b5ee00_TEST PASS\n");
    return 0;
}