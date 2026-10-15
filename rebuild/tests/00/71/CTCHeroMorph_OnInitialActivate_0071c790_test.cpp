#include <cstdio>
// Behaviour of CTCHeroMorph::OnInitialActivate (0x0071C790): Changed set, tattoos initialised, then the texture morphs
// uploaded to the application's engine with (null set, false, 0.0f).
struct CIEngine { int id; };
struct CLongSet;
struct CDisplay_Retail { CIEngine* m_pEngine; };
struct CApplication_Retail { CDisplay_Retail* m_pDisplay; };
CApplication_Retail* g_pApplication = 0;
static int g_order = 0; static int g_tattoos = 0; static int g_upload = 0; static CIEngine* g_engine = 0; static bool g_ok = true;
struct CTCHeroMorph_Retail {
    bool Changed;
    void UploadTextureMorphsToEngine(CIEngine* engine, CLongSet* morphs, bool force, float blend)
    {
        g_upload = ++g_order; g_engine = engine;
        if (morphs != 0 || force || blend != 0.0f || !Changed) g_ok = false;
    }
    void InitialiseCustomTattoos() { g_tattoos = ++g_order; if (!Changed) g_ok = false; }
    void OnInitialActivate()
    {
        Changed = true;
        InitialiseCustomTattoos();
        UploadTextureMorphsToEngine(g_pApplication->m_pDisplay->m_pEngine, 0, false, 0.0f);
    }
};
int main()
{
    CIEngine engine; CDisplay_Retail display; CApplication_Retail app;
    display.m_pEngine = &engine; app.m_pDisplay = &display; g_pApplication = &app;
    CTCHeroMorph_Retail m; m.Changed = false;
    m.OnInitialActivate();
    if (!g_ok || g_tattoos != 1 || g_upload != 2 || g_engine != &engine || !m.Changed) { std::printf("0071c790_TEST FAIL\n"); return 1; }
    std::printf("0071c790_TEST PASS\n");
    return 0;
}