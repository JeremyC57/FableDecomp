// Behaviour test for 0x007A0950: FinalCleanup releases the ability lock only when lightning (0xb) is current, then stops the sound
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SAbilities { bool AbilityMutexLock; long CurrentAbility; };
struct SLightning { SAbilities* Abilities; int Stops;
    void StopLoopingSound() { ++Stops; }
    void FinalCleanup()
    {
        if (Abilities && Abilities->CurrentAbility == 0xb) { Abilities->AbilityMutexLock = false; Abilities->CurrentAbility = 0; }
        StopLoopingSound();
    } };
int main()
{
    SAbilities a; a.AbilityMutexLock = true; a.CurrentAbility = 0xb; SLightning l; l.Abilities = &a; l.Stops = 0;
    l.FinalCleanup(); CHECK(!a.AbilityMutexLock && a.CurrentAbility == 0 && l.Stops == 1);
    a.AbilityMutexLock = true; a.CurrentAbility = 5; l.FinalCleanup(); CHECK(a.AbilityMutexLock && a.CurrentAbility == 5 && l.Stops == 2);
    l.Abilities = 0; l.FinalCleanup(); CHECK(l.Stops == 3);
    if (!g_fail) printf("007a0950_TEST PASS\n");
    return g_fail;
}