// Behaviour test for CTCStealth::SetAsRunningAwayFromCombat (0x007470E0): the def's hang time in frames, or one frame.
#include <cstdio>

long g_FramesPerSecond = 15;

struct CStealthDef {
    float RunningAwayFromCombatHangtimeSeconds;
};

struct CTCStealth {
    CStealthDef* PDef;
    long         RunningAwayFromCombatTimer;
    void SetAsRunningAwayFromCombat();
};

void CTCStealth::SetAsRunningAwayFromCombat()
{
    RunningAwayFromCombatTimer = (long)(PDef ? PDef->RunningAwayFromCombatHangtimeSeconds * g_FramesPerSecond : 1.0f);
}

int main()
{
    CStealthDef def; def.RunningAwayFromCombatHangtimeSeconds = 2.5f;
    CTCStealth s; s.PDef = &def; s.RunningAwayFromCombatTimer = 0;
    s.SetAsRunningAwayFromCombat();
    long withDef = s.RunningAwayFromCombatTimer;
    s.PDef = 0;
    s.SetAsRunningAwayFromCombat();
    if (withDef != 37 || s.RunningAwayFromCombatTimer != 1) { std::printf("007470e0_TEST FAIL\n"); return 1; }
    std::printf("007470e0_TEST PASS\n");
    return 0;
}
