// Behaviour test for 0x00717920 NTCScriptedControl::CActionTalkToThing::OnActivate
#include <cstdio>
struct CPlayerGui { bool CutsceneSkipped; };
static CPlayerGui g_gui;
static CPlayerGui* g_pPlayerGui = &g_gui;
struct CActionTalkToThing {
    bool Terminated; bool AddedDuringCutscene; int baseActivations;
    void BaseOnActivate() { ++baseActivations; }
    void OnActivate()
    {
        if (AddedDuringCutscene && g_pPlayerGui->CutsceneSkipped)
            Terminated = true;
        else
            BaseOnActivate();
    }
};
int main()
{
    CActionTalkToThing a; a.Terminated = false; a.AddedDuringCutscene = true; a.baseActivations = 0;
    g_gui.CutsceneSkipped = true;
    a.OnActivate();
    int ok = a.Terminated && a.baseActivations == 0;
    a.Terminated = false; g_gui.CutsceneSkipped = false;
    a.OnActivate();
    ok = ok && !a.Terminated && a.baseActivations == 1;
    a.AddedDuringCutscene = false; g_gui.CutsceneSkipped = true;
    a.OnActivate();
    ok = ok && !a.Terminated && a.baseActivations == 2;
    if (!ok) { std::printf("00717920_TEST FAIL\n"); return 1; }
    std::printf("00717920_TEST PASS\n");
    return 0;
}