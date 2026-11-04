// Behaviour test for NThingShot::CTCShotDefinitionMultiArrow::InitAttachedEffects (0x007BDD77), g++ model of the retail logic.
#include <cstdio>
struct Thing { bool dead; bool hasEmitter; int attached; };
struct Shot { Thing* effect; Thing self; };
static Thing* g_spawned = 0;
static int g_spawnCount = 0;
static Thing* Create(long type) { ++g_spawnCount; return type == 99 ? 0 : g_spawned; }
static void InitAttachedEffects(Shot& s, long trail)
{
    if (trail) {
        Thing* effect = Create(trail);
        if (effect && !effect->dead) {
            s.effect = effect;
            if (effect->hasEmitter) ++effect->attached;
        }
    }
}
int main()
{
    Thing e = { false, true, 0 };
    g_spawned = &e;
    Shot s = { 0, { false, false, 0 } };
    InitAttachedEffects(s, 0);
    int ok = g_spawnCount == 0 && s.effect == 0;
    InitAttachedEffects(s, 5);
    ok &= s.effect == &e && e.attached == 1;
    Thing dead = { true, true, 0 }; g_spawned = &dead; Shot t = { 0, { false, false, 0 } };
    InitAttachedEffects(t, 5);
    ok &= t.effect == 0 && dead.attached == 0;
    InitAttachedEffects(t, 99);
    ok &= t.effect == 0;
    std::printf(ok ? "007bdd77_TEST PASS\n" : "007bdd77_TEST FAIL\n");
    return ok ? 0 : 1;
}