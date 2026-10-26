// Behaviour test (0x0049ea10): CWorld::SetMiniMapEnabled records whether the minimap was active only on a disable transition
#include <cstdio>
class CPlayerGui { public:
    bool Active;
    int  Calls;
    bool IsMiniMapActive() const { ++const_cast<CPlayerGui*>(this)->Calls; return Active; }
};
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
class CWorld { public:
    unsigned char _pad00[0xdd];
    bool          MiniMapEnabled;
    bool          MiniMapActiveBeforeDisabled;
    void SetMiniMapEnabled(bool enabled)
    {
        if (enabled != MiniMapEnabled)
        {
            MiniMapEnabled = enabled;
            if (!enabled)
                MiniMapActiveBeforeDisabled = g_pPlayerGui->IsMiniMapActive();
        }
    }
};
int main()
{
    CWorld w;
    w.MiniMapEnabled = true; w.MiniMapActiveBeforeDisabled = false;
    g_gui.Active = true; g_gui.Calls = 0;
    w.SetMiniMapEnabled(true);                                   // no change: nothing happens
    if (g_gui.Calls != 0 || !w.MiniMapEnabled) { std::printf("0049ea10_TEST FAIL\n"); return 1; }
    w.SetMiniMapEnabled(false);                                  // disable: remember the gui state
    if (g_gui.Calls != 1 || w.MiniMapEnabled || !w.MiniMapActiveBeforeDisabled) { std::printf("0049ea10_TEST FAIL\n"); return 1; }
    g_gui.Active = false;
    w.SetMiniMapEnabled(true);                                   // enable: gui not queried, flag kept
    if (g_gui.Calls != 1 || !w.MiniMapEnabled || !w.MiniMapActiveBeforeDisabled) { std::printf("0049ea10_TEST FAIL\n"); return 1; }
    w.SetMiniMapEnabled(false);
    if (g_gui.Calls != 2 || w.MiniMapActiveBeforeDisabled) { std::printf("0049ea10_TEST FAIL\n"); return 1; }
    std::printf("0049ea10_TEST PASS\n");
    return 0;
}