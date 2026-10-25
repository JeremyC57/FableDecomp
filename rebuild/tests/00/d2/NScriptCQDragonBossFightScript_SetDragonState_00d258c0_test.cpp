// Behaviour test for CQ_DragonBossFightScript::SetDragonState (0x00D258C0): store the state, then (unless the thread
// is terminating) load that state's minion / summoner targets from the script def; unknown states load nothing.
#include <cstdio>

struct CScriptDef {
    long DBNumMinions[4];
    long DBNumSummoners[4];
};
CScriptDef* g_pScriptDef = 0;
enum EDragonState { DRAGON_HIGH_HEALTH, DRAGON_MEDIUM_HEALTH, DRAGON_LOW_HEALTH, DRAGON_VERY_LOW_HEALTH };
struct CQ_DragonBossFightScript {
    bool terminating;
    EDragonState DragonState;
    long TargetNumMinions;
    long TargetNumSummoners;
    bool IsActiveThreadTerminating() { return terminating; }
    void SetDragonState(EDragonState state)
    {
        DragonState = state;
        if (state == DRAGON_HIGH_HEALTH) {
            if (IsActiveThreadTerminating()) return;
            TargetNumMinions = g_pScriptDef->DBNumMinions[0];
            TargetNumSummoners = g_pScriptDef->DBNumSummoners[0];
        } else if (state == DRAGON_MEDIUM_HEALTH) {
            if (IsActiveThreadTerminating()) return;
            TargetNumMinions = g_pScriptDef->DBNumMinions[1];
            TargetNumSummoners = g_pScriptDef->DBNumSummoners[1];
        } else if (state == DRAGON_LOW_HEALTH) {
            if (IsActiveThreadTerminating()) return;
            TargetNumMinions = g_pScriptDef->DBNumMinions[2];
            TargetNumSummoners = g_pScriptDef->DBNumSummoners[2];
        } else if (state == DRAGON_VERY_LOW_HEALTH) {
            if (IsActiveThreadTerminating()) return;
            TargetNumMinions = g_pScriptDef->DBNumMinions[3];
            TargetNumSummoners = g_pScriptDef->DBNumSummoners[3];
        }
    }
};

int main()
{
    CScriptDef def;
    for (int i = 0; i < 4; ++i) { def.DBNumMinions[i] = 10 + i; def.DBNumSummoners[i] = 20 + i; }
    g_pScriptDef = &def;
    CQ_DragonBossFightScript s;
    s.terminating = false; s.TargetNumMinions = -1; s.TargetNumSummoners = -1;
    int bad = 0;
    for (int st = 0; st < 4; ++st) {
        s.SetDragonState((EDragonState)st);
        if (s.DragonState != st || s.TargetNumMinions != 10 + st || s.TargetNumSummoners != 20 + st) ++bad;
    }
    s.SetDragonState((EDragonState)7);
    if (s.DragonState != 7 || s.TargetNumMinions != 13) ++bad;
    s.terminating = true;
    s.SetDragonState(DRAGON_HIGH_HEALTH);
    if (s.DragonState != DRAGON_HIGH_HEALTH || s.TargetNumMinions != 13 || s.TargetNumSummoners != 23) ++bad;
    if (bad) {
        std::printf("00d258c0_TEST FAIL\n");
        return 1;
    }
    std::printf("00d258c0_TEST PASS\n");
    return 0;
}