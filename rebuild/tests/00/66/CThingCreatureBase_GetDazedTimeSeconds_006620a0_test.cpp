#include <cstdio>
// behaviour test for CThingCreatureBase::GetDazedTimeSeconds (0x006620A0)
static unsigned long g_seed = 0;
static float g_lastRange = -1.0f;
class CWorld { public:
    unsigned long* GetPRandomSeed() { return &g_seed; }
};
static CWorld g_world;
// deterministic stand-in: a random value in [0, range) taken from the seed as a fraction
float GFFloatRandom(float range, unsigned long* seed) { g_lastRange = range; *seed += 1; return range * 0.25f; }
class CFloatRange { public: float Min; float Max; };
class CThingCreatureDef { public:
    unsigned char _pad00[0x70];
    CFloatRange StunTime;
    unsigned char _pad78[0x4];
    CFloatRange DazedTime;
};
class CThingCreatureBase { public:
    unsigned char _pad000[0xe0];
    const CThingCreatureDef* PDef;
    CWorld* GetWorld() { return &g_world; }
    float GetDazedTimeSeconds();
};
float CThingCreatureBase::GetDazedTimeSeconds()
{
    CFloatRange range = PDef->DazedTime;
    return GFFloatRandom(range.Max - range.Min, GetWorld()->GetPRandomSeed()) + range.Min;
}
int main()
{
    static CThingCreatureDef def; static CThingCreatureBase c;
    def.StunTime.Min = 2.0f; def.StunTime.Max = 6.0f;
    def.DazedTime.Min = 2.0f; def.DazedTime.Max = 6.0f;
    c.PDef = &def;
    float v = c.GetDazedTimeSeconds();
    if (v != 3.0f || g_lastRange != 4.0f || g_seed != 1) { std::printf("006620a0_TEST FAIL\n"); return 1; }
    std::printf("006620a0_TEST PASS\n");
    return 0;
}