// Behaviour test for CGameCameraManager::SetScriptedCameraMode (0x00699E60), a portable model of the row: with the scripted-camera
// preload flag set the module is only preloaded (ScriptHasPreloadedCamera = true); otherwise it becomes the pending module (reference
// counted), ScriptedCameraModeFromGame = false, the remove timer -1, the preload flags follow the global preload switch / an earlier
// preload / the current mode type (8 = no preload), a new scripted-control mode holding the pending module is set as the camera mode,
// and the cleanup flags are updated; the by-value argument's reference is released on every path.
#include <stdio.h>
struct Info { long RefCount; };
struct Module { Info* info; };
static bool g_Preload = false;
struct Manager {
    bool PreloadFlag, HasPreloaded, Preload, FastMode, FromGame; long Timer; long CurrentType;
    Info* Pending; int Preloads; int ModesSet; Info* ModeModule; int Cleanups;
    void SetScriptedCameraMode(Module m)
    {
        if (m.info) ++m.info->RefCount;                          // the by-value copy
        if (PreloadFlag) {
            ++Preloads; HasPreloaded = true;
        } else {
            if (Pending != m.info) { if (Pending) --Pending->RefCount; Pending = m.info; if (Pending) ++Pending->RefCount; }
            FromGame = false; Timer = -1;
            if (g_Preload) { Preload = true; FastMode = false; if (CurrentType == 8) FastMode = true; }
            else if (HasPreloaded) { Preload = false; HasPreloaded = false; }
            else if (CurrentType != 8) { Preload = true; FastMode = false; }
            ModeModule = Pending; if (ModeModule) ++ModeModule->RefCount; ++ModesSet;   // CScriptedControl keeps a reference
            ++Cleanups;
        }
        if (m.info) --m.info->RefCount;                          // the argument's release
    }
};
static Manager Fresh() { Manager g = { false, false, false, true, true, 0, 3, 0, 0, 0, 0, 0 }; return g; }
int main()
{
    int bad = 0;
    Info a = { 1 }; Module m = { &a };
    Manager g = Fresh(); g.PreloadFlag = true;
    g.SetScriptedCameraMode(m);
    if (g.Preloads != 1 || !g.HasPreloaded || g.ModesSet != 0 || g.Pending != 0 || a.RefCount != 1) ++bad;
    g = Fresh(); g_Preload = true; g.CurrentType = 8;
    g.SetScriptedCameraMode(m);
    if (!g.Preload || !g.FastMode || g.Timer != -1 || g.FromGame || g.Pending != &a || a.RefCount != 3 || g.ModesSet != 1 || g.Cleanups != 1) ++bad;
    Info b = { 1 }; Module mb = { &b }; g_Preload = false; g.HasPreloaded = true;
    g.SetScriptedCameraMode(mb);                                 // replaces the pending module: a drops its pending reference
    if (g.Preload || g.HasPreloaded || g.Pending != &b || a.RefCount != 2 || b.RefCount != 3) ++bad;
    g = Fresh(); g.CurrentType = 2;
    g.SetScriptedCameraMode(m);
    if (!g.Preload || g.FastMode) ++bad;
    g = Fresh(); g.CurrentType = 8;
    g.SetScriptedCameraMode(m);
    if (g.Preload || !g.FastMode) ++bad;                         // mode 8 without the switch: flags untouched
    printf(bad ? "00699e60_TEST FAIL %d\n" : "00699e60_TEST PASS\n", bad);
    return bad != 0;
}
