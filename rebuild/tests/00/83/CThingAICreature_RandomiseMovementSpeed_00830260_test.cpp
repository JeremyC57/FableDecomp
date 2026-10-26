// 00830260 behaviour test: CThingAICreature::RandomiseMovementSpeed (final.cpp body with deterministic callee models)
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
typedef unsigned int u32;   // 32-bit unsigned on both hosts
static float GFFloatRandom(float range, u32* seed) { *seed = *seed * 3u + 1u; return range * 0.5f; }
static int g_lastRor;
static int GFROR13(u32 v) { g_lastRor = (int)((v >> 13) | (v << 19)); return g_lastRor; }
class CCreatureDef { public: float m_speedVariation; };
class CThingAICreature { public:
    u32 m_uniqueIdLow; CCreatureDef* m_pCreatureDef;
    void RandomiseMovementSpeed(float* speed);
};
void CThingAICreature::RandomiseMovementSpeed(float* speed)
{
    u32 seed = m_uniqueIdLow;
    float delta = GFFloatRandom(*speed * m_pCreatureDef->m_speedVariation, &seed);
    if (!(GFROR13(seed * 0x24a1 + 0x24df) & 1))
        delta = -delta;
    *speed = delta + *speed;
}
static int odd(u32 id) { u32 s = id * 3u + 1u; u32 v = s * 0x24a1u + 0x24dfu; return (int)(((v >> 13) | (v << 19)) & 1); }
int main()
{
    CCreatureDef def = { 0.5f };
    int seenPlus = 0, seenMinus = 0;
    for (u32 id = 1; id < 64; ++id) {
        CThingAICreature c = { id, &def };
        float speed = 4.0f;
        c.RandomiseMovementSpeed(&speed);
        if (odd(id)) { CHECK(speed == 5.0f); ++seenPlus; } else { CHECK(speed == 3.0f); ++seenMinus; }
        CHECK(c.m_uniqueIdLow == id);
    }
    CHECK(seenPlus > 0 && seenMinus > 0);
    if (!g_fail) printf("00830260_TEST PASS\n");
    return g_fail;
}