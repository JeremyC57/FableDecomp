#include <cstdio>
struct CCombatAbilityData_Calls { void* __vftable; long PercentageChance; float SecondsDuration; float SecondsToWaitBeforeRepeat; };
long g_framesPerSecond = 30;
struct CCombatAbility_Retail {
    long RepeatTimer; long ActiveTimer;
    void Update(const CCombatAbilityData_Calls& data)
    {
        if (RepeatTimer >= 0) --RepeatTimer;
        if (ActiveTimer >= 0) { if (--ActiveTimer == -1) RepeatTimer = (long)(data.SecondsToWaitBeforeRepeat * g_framesPerSecond); }
    }
};
int main()
{
    CCombatAbilityData_Calls data = { 0, 50, 1.0f, 2.5f };
    CCombatAbility_Retail a; a.RepeatTimer = 5; a.ActiveTimer = 1;
    a.Update(data); if (a.RepeatTimer != 4 || a.ActiveTimer != 0) { std::printf("00733360_TEST FAIL\n"); return 1; }
    a.Update(data); if (a.RepeatTimer != 75 || a.ActiveTimer != -1) { std::printf("00733360_TEST FAIL\n"); return 1; }
    a.Update(data); if (a.RepeatTimer != 74 || a.ActiveTimer != -1) { std::printf("00733360_TEST FAIL\n"); return 1; }
    a.RepeatTimer = -1; a.ActiveTimer = -3; a.Update(data); if (a.RepeatTimer != -1 || a.ActiveTimer != -3) { std::printf("00733360_TEST FAIL\n"); return 1; }
    std::printf("00733360_TEST PASS\n"); return 0;
}