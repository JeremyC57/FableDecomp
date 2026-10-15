#include <cstdio>
// Behaviour test for 0048de44: CSubtitleRenderer members used by the body (retail layout names from the PDB), with
// recording stand-ins for the out-of-line callees.
struct C2DVector { float X, Y; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank { public: int m_id; };
struct CFadeInFadeOutBase {
    bool  Active;
    float FadeInTime;
    float FadeOutTime;
    bool  Closing;
    bool  Opening;
    float OpenTimer;
    float CloseTimer;
};
struct CDisplayEngine_0048de44 { long m_busy; };
struct CGameComponent_0048de44 { CDisplayEngine_0048de44* m_pDisplayEngine; };
static CDisplayEngine_0048de44 g_engine;
static CGameComponent_0048de44 g_component = { &g_engine };
static CGameComponent_0048de44* g_pGameComponent = &g_component;
class CSubtitleRenderer {
public:
    unsigned long      DisplayFlags;
    bool               InCutscene;
    bool               CutsceneSkipped;
    bool               CutsceneSkippable;
    bool               CutsceneActionMode;
    bool               DisplayingText;
    CFadeInFadeOutBase SkipCutsceneTimer;
    bool IsCutsceneSkippable() const;
};
bool CSubtitleRenderer::IsCutsceneSkippable() const
{
    if (g_pGameComponent->m_pDisplayEngine->m_busy != 0)
        return false;
    if (InCutscene) {
        if (!CutsceneSkippable)
            return false;
        if (CutsceneSkipped)
            return false;
        if (DisplayFlags & 0x48)
            return false;
        if (!SkipCutsceneTimer.Active)
            return false;
        if (SkipCutsceneTimer.Opening)
            return false;
    }
    return true;
}
#define FAIL do { std::printf("0048de44_TEST FAIL\n"); return 1; } while (0)
int main()
{
    CSubtitleRenderer r; r.InCutscene = false; r.CutsceneSkippable = false; r.CutsceneSkipped = false; r.DisplayFlags = 0;
    r.SkipCutsceneTimer.Active = false; r.SkipCutsceneTimer.Opening = false;
    g_engine.m_busy = 1; if (r.IsCutsceneSkippable()) FAIL;
    g_engine.m_busy = 0; if (!r.IsCutsceneSkippable()) FAIL;
    r.InCutscene = true; if (r.IsCutsceneSkippable()) FAIL;
    r.CutsceneSkippable = true; if (r.IsCutsceneSkippable()) FAIL;
    r.SkipCutsceneTimer.Active = true; if (!r.IsCutsceneSkippable()) FAIL;
    r.SkipCutsceneTimer.Opening = true; if (r.IsCutsceneSkippable()) FAIL;
    r.SkipCutsceneTimer.Opening = false; r.DisplayFlags = 0x40; if (r.IsCutsceneSkippable()) FAIL;
    r.DisplayFlags = 0x4; if (!r.IsCutsceneSkippable()) FAIL;
    r.CutsceneSkipped = true; if (r.IsCutsceneSkippable()) FAIL;
    std::printf("0048de44_TEST PASS\n");
    return 0;
}
