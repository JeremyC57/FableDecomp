// Portable behaviour test for CTCFineDialogue::IsAllowed (0x007E2790), with the retail member names of the accessors it inlines:
// CDisplayEngine::IsScreenFadeOut (ScreenFadeOutInfo.Active), CSubtitleRenderer::IsDisplayingText / IsInCutscene and
// CWorld::IsPerformingRegionLoad (RegionLoadStatus != 0 && != 9).
#include <cstdio>
struct CFadeInFadeOutBase { bool Active; bool IsActive() const { return Active; } };
class CDisplayEngine { public: CFadeInFadeOutBase ScreenFadeOutInfo; bool IsScreenFadeOut() const { return ScreenFadeOutInfo.IsActive(); } };
class CMainGameComponent { public: CDisplayEngine* PDisplayEngine; const CDisplayEngine& PeekDisplayEngine() const { return *PDisplayEngine; } };
class CSubtitleRenderer { public: bool InCutscene; bool DisplayingText;
    bool IsDisplayingText() const { return DisplayingText; } bool IsInCutscene() const { return InCutscene; } };
class CPlayerGui { public: CSubtitleRenderer SubtitleRenderer; static CPlayerGui* PClass;
    static CPlayerGui& Get() { return *PClass; } CSubtitleRenderer& GetSubtitleRenderer() { return SubtitleRenderer; } };
class CWorld { public: long RegionLoadStatus; bool IsPerformingRegionLoad() const { return RegionLoadStatus != 0 && RegionLoadStatus != 9; } };
static CDisplayEngine g_display; static CMainGameComponent g_component; static CPlayerGui g_gui; static CWorld g_world;
CPlayerGui* CPlayerGui::PClass = &g_gui;
class CTCBase { protected: const CMainGameComponent& PeekComponent() const { return g_component; } CWorld& GetWorld() const { return g_world; } };
class CTCFineDialogue : public CTCBase { public:
    long Fine; long Money; bool GivePlayerChanceToPayFine; bool WaitingForConfirmation;
    bool IsAllowed()
    {
        if (!GivePlayerChanceToPayFine) return false;
        if (PeekComponent().PeekDisplayEngine().IsScreenFadeOut()) return false;
        if (CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) return false;
        if (CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) return false;
        if (GetWorld().IsPerformingRegionLoad()) return false;
        return true;
    }
};
int main()
{
    g_component.PDisplayEngine = &g_display; g_display.ScreenFadeOutInfo.Active = false;
    g_gui.SubtitleRenderer.InCutscene = false; g_gui.SubtitleRenderer.DisplayingText = false; g_world.RegionLoadStatus = 0;
    CTCFineDialogue d; d.GivePlayerChanceToPayFine = false;
    bool r0 = d.IsAllowed();
    d.GivePlayerChanceToPayFine = true; bool r1 = d.IsAllowed();
    g_world.RegionLoadStatus = 9; bool r2 = d.IsAllowed();
    g_world.RegionLoadStatus = 4; bool r3 = d.IsAllowed();
    g_world.RegionLoadStatus = 0; g_display.ScreenFadeOutInfo.Active = true; bool r4 = d.IsAllowed();
    g_display.ScreenFadeOutInfo.Active = false; g_gui.SubtitleRenderer.DisplayingText = true; bool r5 = d.IsAllowed();
    g_gui.SubtitleRenderer.DisplayingText = false; g_gui.SubtitleRenderer.InCutscene = true; bool r6 = d.IsAllowed();
    if (r0 || !r1 || !r2 || r3 || r4 || r5 || r6) { std::printf("007e2790_TEST FAIL\n"); return 1; }
    std::printf("007e2790_TEST PASS\n");
    return 0;
}
