// Behaviour test for CCreatureAction_BlockRespond::FrameUpdate (0x008553C0): on the first running frame, when combat particles
// apply between the creature and its target, a dust effect is created along MovementDirection; the base FrameUpdate always runs.
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CThing { public: int Id; };
static int g_dustCalls, g_baseCalls, g_particleQueries;
static const CThing* g_dustCreature;
static const C3DVector* g_dustDirection;
static void* g_dustHits;
static bool g_allowParticles;
static bool IsToCreateCombatParticleEffects(const CThing& creature, const CThing* target)
{
    ++g_particleQueries;
    return g_allowParticles && target != 0 && creature.Id != target->Id;
}
static void CreateDustEffectForCreature(CThing& creature, const int& worldMap, const int& defs, const C3DVector& direction, void* hits)
{
    (void)worldMap; (void)defs;
    ++g_dustCalls; g_dustCreature = &creature; g_dustDirection = &direction; g_dustHits = hits;
}
struct CPointer { CThing* Item; CThing* Get() const { return Item; } CThing& operator*() const { return *Item; } };
class CCombatActionBase { public:
    CPointer PCreature;
    long FramesRunning;
    CPointer PTarget;
    int WorldMap, Defs;
    int& GetWorldMap() { return WorldMap; }
    const int& PeekDefinitionManager() const { return Defs; }
    void BaseFrameUpdate() { ++g_baseCalls; }
};
class CCreatureAction_BlockRespond : public CCombatActionBase { public:
    long BlowStrength;
    C3DVector MovementDirection;
    void FrameUpdate()
    {
        if (FramesRunning == 1) {
            CThing* creature = PCreature.Get();
            if (IsToCreateCombatParticleEffects(*creature, PTarget.Get()))
                CreateDustEffectForCreature(*PCreature, GetWorldMap(), PeekDefinitionManager(), MovementDirection, 0);
        }
        BaseFrameUpdate();
    }
};
int main()
{
    CThing hero = { 1 }, enemy = { 2 };
    CCreatureAction_BlockRespond a;
    a.PCreature.Item = &hero; a.PTarget.Item = &enemy; a.MovementDirection.X = 1.0f;
    int fail = 0;
    a.FramesRunning = 1; g_allowParticles = true; a.FrameUpdate();
    if (g_dustCalls != 1 || g_dustCreature != &hero || g_dustDirection != &a.MovementDirection || g_dustHits != 0 || g_baseCalls != 1) fail = 1;
    g_allowParticles = false; a.FrameUpdate();
    if (g_dustCalls != 1 || g_baseCalls != 2 || g_particleQueries != 2) fail = 1;
    a.FramesRunning = 2; g_allowParticles = true; a.FrameUpdate();
    if (g_dustCalls != 1 || g_baseCalls != 3 || g_particleQueries != 2) fail = 1;
    if (fail) { std::printf("008553c0_TEST FAIL\n"); return 1; }
    std::printf("008553c0_TEST PASS\n");
    return 0;
}