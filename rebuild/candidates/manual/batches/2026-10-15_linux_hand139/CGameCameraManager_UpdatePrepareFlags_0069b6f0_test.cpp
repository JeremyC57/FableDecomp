// Behaviour test for 0x0069B6F0 CGameCameraManager::UpdatePrepareFlags.
#include <cstdio>
static int g_push = 0, g_melee = 0, g_building = 0, g_release = 0;
static bool g_waiting = false;
struct Mode { bool finished; bool IsFinished() { return finished; } };
struct M {
    Mode* CurrentMode; bool NewAutoMode; Mode* PCurrentScriptModule; Mode* PPendingScriptModule; int ScriptedCameraRemoveTimer;
    bool UsingCameraManually;
    void PushCurrentManualMode() { ++g_push; }
    void ReleaseScriptedControl() { ++g_release; }
    void UpdatePrepareFlags()
    {
        if (CurrentMode && CurrentMode->IsFinished()) { if (!UsingCameraManually) NewAutoMode = true; else PushCurrentManualMode(); }
        ++g_melee; ++g_building;
        if (!PPendingScriptModule) {
            bool waitingForSubtitle = g_waiting;
            if (PCurrentScriptModule && PCurrentScriptModule->IsFinished() && !waitingForSubtitle) {
                if (ScriptedCameraRemoveTimer < 0) ScriptedCameraRemoveTimer = 1;
                if (ScriptedCameraRemoveTimer == 0) ReleaseScriptedControl();
                --ScriptedCameraRemoveTimer;
            }
        }
    }
};
static int Fail() { std::printf("0069b6f0_TEST FAIL\n"); return 1; }
int main()
{
    Mode cur; cur.finished = true; Mode script; script.finished = true;
    M m; m.CurrentMode = &cur; m.NewAutoMode = false; m.PCurrentScriptModule = &script; m.PPendingScriptModule = 0;
    m.ScriptedCameraRemoveTimer = -1; m.UsingCameraManually = false;
    m.UpdatePrepareFlags();
    if (!m.NewAutoMode || g_melee != 1 || g_building != 1 || m.ScriptedCameraRemoveTimer != 0 || g_release) return Fail();
    m.UpdatePrepareFlags();
    if (g_release != 1 || m.ScriptedCameraRemoveTimer != -1) return Fail();
    g_waiting = true; m.UsingCameraManually = true; m.UpdatePrepareFlags();
    if (g_push != 1 || m.ScriptedCameraRemoveTimer != -1) return Fail();
    std::printf("0069b6f0_TEST PASS\n");
    return 0;
}
