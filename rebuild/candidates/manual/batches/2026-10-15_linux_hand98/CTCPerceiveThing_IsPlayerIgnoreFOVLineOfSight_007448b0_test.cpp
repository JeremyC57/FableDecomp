#include <cstdio>
struct CThing { unsigned char Type; };
static int g_getCalls = 0;
struct CBaseIntelligentPointer { CThing* p; CThing* GetPItem() const { ++g_getCalls; return p; } };
struct CTCPerceiveThing {
    CBaseIntelligentPointer PerceivedThing;
    bool IgnoreFOVLineOfSight;
    long IsPlayerIgnoreFOVLineOfSight() const
    {
        const CBaseIntelligentPointer* perceived = &PerceivedThing;
        if (perceived->GetPItem()) {
            if (perceived->GetPItem()->Type == 2) {
                if (IgnoreFOVLineOfSight)
                    return 1;
            }
        }
        return 0;
    }
};
int main()
{
    CThing hero; hero.Type = 2; CThing villager; villager.Type = 1;
    CTCPerceiveThing p; p.PerceivedThing.p = 0; p.IgnoreFOVLineOfSight = true;
    if (p.IsPlayerIgnoreFOVLineOfSight() != 0 || g_getCalls != 1) { std::printf("007448b0_TEST FAIL\n"); return 1; }
    p.PerceivedThing.p = &villager; if (p.IsPlayerIgnoreFOVLineOfSight() != 0) { std::printf("007448b0_TEST FAIL\n"); return 1; }
    p.PerceivedThing.p = &hero; if (p.IsPlayerIgnoreFOVLineOfSight() != 1) { std::printf("007448b0_TEST FAIL\n"); return 1; }
    p.IgnoreFOVLineOfSight = false; if (p.IsPlayerIgnoreFOVLineOfSight() != 0) { std::printf("007448b0_TEST FAIL\n"); return 1; }
    std::printf("007448b0_TEST PASS\n");
    return 0;
}
