// Behaviour test for 007aee90 CTCBoss::IsReadyForPhaseChange: enabled + physical parent + Health below the phase threshold.
#include <cstdio>

struct CThing { unsigned long ThingTypeFlags; };
struct CThingPhysical : CThing { float MaxHealth; float Health; };

struct CTCBoss
{
    CThing* ParentThing;
    long    currentBossPhase;
    bool    EnablePhaseChanges;
    float   m_threshold[2];                         // stands in for the def lookup of 0x007AEE40

    float GetMinHealthForPhase(long phase) const { return m_threshold[phase]; }
    bool IsReadyForPhaseChange() const
    {
        if (EnablePhaseChanges)
        {
            CThing* thing = ParentThing;
            if (thing->ThingTypeFlags & 4)
            {
                if (GetMinHealthForPhase(currentBossPhase) > static_cast<CThingPhysical*>(thing)->Health)
                    return true;
            }
        }
        return false;
    }
};

int main()
{
    CThingPhysical thing; thing.ThingTypeFlags = 4; thing.MaxHealth = 100.0f; thing.Health = 60.0f;
    CTCBoss boss; boss.ParentThing = &thing; boss.currentBossPhase = 1; boss.EnablePhaseChanges = true;
    boss.m_threshold[0] = 75.0f; boss.m_threshold[1] = 50.0f;
    if (boss.IsReadyForPhaseChange()) { std::printf("007aee90_TEST FAIL\n"); return 1; }
    boss.currentBossPhase = 0;
    if (!boss.IsReadyForPhaseChange()) { std::printf("007aee90_TEST FAIL\n"); return 1; }
    thing.Health = 75.0f;                            // equal is not below
    if (boss.IsReadyForPhaseChange()) { std::printf("007aee90_TEST FAIL\n"); return 1; }
    thing.Health = 10.0f; boss.EnablePhaseChanges = false;
    if (boss.IsReadyForPhaseChange()) { std::printf("007aee90_TEST FAIL\n"); return 1; }
    boss.EnablePhaseChanges = true; thing.ThingTypeFlags = 0;
    if (boss.IsReadyForPhaseChange()) { std::printf("007aee90_TEST FAIL\n"); return 1; }
    std::printf("007aee90_TEST PASS\n");
    return 0;
}
