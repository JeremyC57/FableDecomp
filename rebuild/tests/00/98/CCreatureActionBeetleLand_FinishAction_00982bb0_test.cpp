// behaviour test for 0x00982BB0 CCreatureAction_BeetleLand::FinishAction() (model; the component lookup and the mode manager are stubs)
#include <cstdio>
static int g_calls = 0, g_base = 0; static unsigned long g_mode = 0;
class CTCCreatureModeManager { public: void RemoveMode(unsigned long m) { ++g_calls; g_mode = m; } };
class CThing { public: CTCCreatureModeManager* modes; bool GetInterface(CTCCreatureModeManager** out, long type) { if (type != 0x31 || !modes) return false; *out = modes; return true; } };
struct CAction {
    CThing* creature;
    CThing* GetCreature() const { return creature; }
    void BaseFinish() { ++g_base; }
    void FinishAction()
    {
        CTCCreatureModeManager* modes;
        if (GetCreature()->GetInterface(&modes, 0x31))
            modes->RemoveMode(0x1A);
        BaseFinish();
    }
};
int main()
{
    CTCCreatureModeManager mgr; CThing t; t.modes = &mgr; CAction a; a.creature = &t;
    a.FinishAction();
    int ok = g_calls == 1 && g_mode == 0x1A && g_base == 1;
    t.modes = 0; a.FinishAction();
    ok = ok && g_calls == 1 && g_base == 2;
    if (!ok) { std::printf("00982bb0_TEST FAIL\n"); return 1; }
    std::printf("00982bb0_TEST PASS\n");
    return 0;
}