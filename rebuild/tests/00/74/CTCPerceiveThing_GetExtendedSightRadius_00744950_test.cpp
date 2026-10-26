#include <cstdio>
class CGameTimeManager { public: int phase; bool IsDay() const { return phase == 0; } bool IsNightTime() const { return phase == 1; } };
class CWorld { public: CGameTimeManager* GameTimeManager; };
static CGameTimeManager g_time;
static CWorld g_world;
class CPerceivedThingDef { public: float DayTimeVisibilityMultiplier; float NightTimeVisibilityMultiplier; };
class CTCPerceiveThing { public:
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
    float GetExtendedSightRadius() const { return GetVisibilityMultiplier() * ExtendedSightRadius; }
};
int main()
{
    g_world.GameTimeManager = &g_time;
    CPerceivedThingDef d; d.DayTimeVisibilityMultiplier = 1.0f; d.NightTimeVisibilityMultiplier = 0.5f;
    CTCPerceiveThing p; p.PDef_Object = &d; p.ExtendedSightRadius = 20.0f;
    g_time.phase = 0; if (p.GetExtendedSightRadius() != 20.0f) { std::printf("00744950_TEST FAIL\n"); return 1; }
    g_time.phase = 1; if (p.GetExtendedSightRadius() != 10.0f) { std::printf("00744950_TEST FAIL\n"); return 1; }
    g_time.phase = 2; if (p.GetExtendedSightRadius() != 15.0f) { std::printf("00744950_TEST FAIL\n"); return 1; }
    std::printf("00744950_TEST PASS\n");
    return 0;
}