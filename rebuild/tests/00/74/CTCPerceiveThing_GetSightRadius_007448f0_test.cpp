#include <cstdio>
struct CGameTimeManager { int phase; bool IsDay() const { return phase == 0; } bool IsNightTime() const { return phase == 1; } };
struct CWorld { CGameTimeManager* GameTimeManager; };
static CGameTimeManager g_time;
static CWorld g_world;
struct CPerceivedThingDef { float DayTimeVisibilityMultiplier; float NightTimeVisibilityMultiplier; };
struct CTCPerceiveThing {
    const CPerceivedThingDef* PDef_Object;
    float SightRadius;
    float ExtendedSightRadius;
    CWorld* GetWorld() const { return &g_world; }
    float GetVisibilityMultiplier() const
    {
        if (GetWorld()->GameTimeManager->IsDay())
            return PDef_Object->DayTimeVisibilityMultiplier;
        if (GetWorld()->GameTimeManager->IsNightTime())
            return PDef_Object->NightTimeVisibilityMultiplier;
        return (PDef_Object->NightTimeVisibilityMultiplier + PDef_Object->DayTimeVisibilityMultiplier) * 0.5f;
    }
    float GetSightRadius() const { return GetVisibilityMultiplier() * SightRadius; }
};
int main()
{
    g_world.GameTimeManager = &g_time;
    CPerceivedThingDef d; d.DayTimeVisibilityMultiplier = 1.0f; d.NightTimeVisibilityMultiplier = 0.5f;
    CTCPerceiveThing p; p.PDef_Object = &d; p.SightRadius = 20.0f;
    g_time.phase = 0; if (p.GetSightRadius() != 20.0f) { std::printf("007448f0_TEST FAIL\n"); return 1; }
    g_time.phase = 1; if (p.GetSightRadius() != 10.0f) { std::printf("007448f0_TEST FAIL\n"); return 1; }
    g_time.phase = 2; if (p.GetSightRadius() != 15.0f) { std::printf("007448f0_TEST FAIL\n"); return 1; }
    std::printf("007448f0_TEST PASS\n");
    return 0;
}