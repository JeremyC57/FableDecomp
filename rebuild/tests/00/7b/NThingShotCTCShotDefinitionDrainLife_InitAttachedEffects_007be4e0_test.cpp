// Behaviour test for the shot InitAttachedEffects (007be4e0), g++ model of the retail logic.
#include <cstdio>
struct V { float X, Y, Z; };
struct Thing { bool dead; bool hasEmitter; int attached; unsigned long key; V dir; };
struct Shot { Thing* effect; V facing; };
static Thing* g_spawned = 0;
static void InitAttachedEffects(Shot& s, long effectType)
{
    if (effectType) {
        Thing* effect = g_spawned;
        if (effect && !effect->dead) {
            s.effect = effect;
            if (effect->hasEmitter) { ++effect->attached; effect->key = 0x1234; effect->dir = s.facing; }
        }
    }
}
int main()
{
    Thing e = { false, true, 0, 0, { 0, 0, 0 } };
    g_spawned = &e;
    Shot s = { 0, { 1, 2, 3 } };
    InitAttachedEffects(s, 0);
    int ok = s.effect == 0 && e.attached == 0;
    InitAttachedEffects(s, 7);
    ok &= s.effect == &e && e.attached == 1 && e.key == 0x1234 && e.dir.X == 1 && e.dir.Y == 2 && e.dir.Z == 3;
    Thing dead = { true, true, 0, 0, { 0, 0, 0 } }; g_spawned = &dead; Shot t = { 0, { 0, 0, 0 } };
    InitAttachedEffects(t, 7);
    ok &= t.effect == 0 && dead.attached == 0;
    std::printf(ok ? "007be4e0_TEST PASS\n" : "007be4e0_TEST FAIL\n");
    return ok ? 0 : 1;
}