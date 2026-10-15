// Behaviour test for 0x0078BFC0 CTCBerserk::ShouldAttackDecapitate: a roll in [0,1) from the world seed is
// compared with the chance for the current spell level; 1 when the roll is at or below it.
#include <cstdio>

static float g_roll;
static float g_range;
static unsigned long* g_seedSeen;
struct CWorld { unsigned long m_seed; unsigned long* GetPRandomSeed() { return &m_seed; } };
static CWorld g_world;
static float GFFloatRandom(float range, unsigned long* seed) { g_range = range; g_seedSeen = seed; return g_roll; }

struct CSpecialAbilitiesBerserkDef { float* DecapitateChances; };
struct CTCBerserk {
    long SpellLevel;
    CSpecialAbilitiesBerserkDef* PDef;
    CWorld* GetWorld() { return &g_world; }
    long ShouldAttackDecapitate()
    {
        float* chance = &PDef->DecapitateChances[SpellLevel];
        if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) <= *chance)
            return 1;
        return 0;
    }
};

int main()
{
    float chances[3] = { 0.1f, 0.5f, 0.9f };
    CSpecialAbilitiesBerserkDef def; def.DecapitateChances = chances;
    CTCBerserk b; b.PDef = &def; b.SpellLevel = 1;
    g_roll = 0.25f;
    if (b.ShouldAttackDecapitate() != 1 || g_range != 1.0f || g_seedSeen != &g_world.m_seed) { std::printf("0078bfc0_TEST FAIL\n"); return 1; }
    g_roll = 0.5f;
    if (b.ShouldAttackDecapitate() != 1) { std::printf("0078bfc0_TEST FAIL\n"); return 1; }
    g_roll = 0.75f;
    if (b.ShouldAttackDecapitate() != 0) { std::printf("0078bfc0_TEST FAIL\n"); return 1; }
    b.SpellLevel = 2;
    if (b.ShouldAttackDecapitate() != 1) { std::printf("0078bfc0_TEST FAIL\n"); return 1; }
    std::printf("0078bfc0_TEST PASS\n");
    return 0;
}