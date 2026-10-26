#include <cstdio>
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
class C2DBoxF { public: float left, top, right, bottom; };
static const C2DBoxF* g_window = 0;
struct CRenderManagerCore { void SetAWindow(const C2DBoxF& w) { g_window = &w; Log('W'); } };
class CDisplayManager { public:
    CRenderManagerCore* PRenderManager;
    bool SetRenderTargetAsBackScreen() { Log('B'); return true; }
    void EndRender() { Log('E'); }
};
class CSystemManager { public: CDisplayManager* PDisplayManager; };
static CSystemManager g_system;
static CSystemManager* GFGetSystemManager() { Log('G'); return &g_system; }
class CRenderTarget { public: void DetachTarget() { Log('D'); } };
class CRenderStateManager { public: void RestoreCaptureBlock() { Log('R'); } };
static CRenderStateManager g_rsm; static CRenderStateManager* g_pRenderStateManager = &g_rsm;
struct CSurfaceCompositor {
    bool m_composing, m_wasRendering; CRenderTarget* m_pRenderTarget; C2DBoxF m_savedWindow;
    void EndComposition()
    {
        CDisplayManager* display = GFGetSystemManager()->PDisplayManager;
        CRenderManagerCore* core = GFGetSystemManager()->PDisplayManager->PRenderManager;
        display->SetRenderTargetAsBackScreen();
        m_pRenderTarget->DetachTarget();
        if (!m_wasRendering)
            display->EndRender();
        m_composing = false;
        g_pRenderStateManager->RestoreCaptureBlock();
        core->SetAWindow(m_savedWindow);
    }
};
static bool Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
int main()
{
    CRenderManagerCore core; CDisplayManager dm; dm.PRenderManager = &core; g_system.PDisplayManager = &dm;
    CRenderTarget rt;
    CSurfaceCompositor c; c.m_composing = true; c.m_wasRendering = false; c.m_pRenderTarget = &rt;
    c.EndComposition();
    if (!Same(g_log, "GGBDERW") || c.m_composing || g_window != &c.m_savedWindow) { std::printf("00f38930_TEST FAIL\n"); return 1; }
    g_n = 0; c.m_composing = true; c.m_wasRendering = true;
    c.EndComposition();
    if (!Same(g_log, "GGBDRW") || c.m_composing) { std::printf("00f38930_TEST FAIL\n"); return 1; }
    std::printf("00f38930_TEST PASS\n");
    return 0;
}