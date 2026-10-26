// Behaviour test for IsCutsceneSkipped (0x00CBEB7E): with the action-button option the action-mode button
// message decides, otherwise the cutscene-skipped message does.
#include <cstdio>

class CGameScriptInterface { public:
    bool actionPressed;
    bool cutsceneSkipped;
    int actionQueries;
    int skipQueries;
    bool MsgIsActionModeButtonPressed() { ++actionQueries; return actionPressed; }
    bool MsgIsCutSceneSkipped() { ++skipQueries; return cutsceneSkipped; }
};
bool g_bSkipCutsceneWithActionButton = false;
CGameScriptInterface* g_pGameScriptInterface = 0;

bool IsCutsceneSkipped()
{
    if (g_bSkipCutsceneWithActionButton) {
        if (g_pGameScriptInterface->MsgIsActionModeButtonPressed())
            return true;
    } else {
        if (g_pGameScriptInterface->MsgIsCutSceneSkipped())
            return true;
    }
    return false;
}

int main()
{
    CGameScriptInterface gsi;
    gsi.actionQueries = 0;
    gsi.skipQueries = 0;
    g_pGameScriptInterface = &gsi;
    int bad = 0;
    for (int mode = 0; mode < 2; ++mode)
        for (int a = 0; a < 2; ++a)
            for (int s = 0; s < 2; ++s) {
                g_bSkipCutsceneWithActionButton = mode != 0;
                gsi.actionPressed = a != 0;
                gsi.cutsceneSkipped = s != 0;
                bool expected = mode ? (a != 0) : (s != 0);
                if (IsCutsceneSkipped() != expected) ++bad;
            }
    if (bad || gsi.actionQueries != 4 || gsi.skipQueries != 4) {
        std::printf("00cbeb7e_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbeb7e_TEST PASS\n");
    return 0;
}