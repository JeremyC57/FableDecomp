// Behaviour test for CTCKraken::StartBeamEffect (0x007FB6A0): with a special-effects sub-def that knows BEAM_ATTACK, the
// effect is created at the mouth position, kept as the beam effect and (when alive with an emitter) attached to the kraken.
#include <cstdio>
#include <cstring>

struct Effect { float x, y, z; bool dead, hasEmitter, attached; };
struct World { bool hasSubDef; const char* known; Effect pool[4]; int created; };
static Effect* CreateEffect(World& w, float x, float y, float z)
{
    Effect* e = &w.pool[w.created++];
    e->x = x; e->y = y; e->z = z; e->attached = false;
    return e;
}
static bool GetEffectFromString(const World& w, const char* name, long& type)
{
    if (std::strcmp(name, w.known) != 0) return false;
    type = 7;
    return true;
}
struct Kraken { Effect* beam; float mx, my, mz; };
static void StartBeamEffect(Kraken& k, World& w)
{
    long effectType = 0;
    if (w.hasSubDef && GetEffectFromString(w, "BEAM_ATTACK", effectType)) {
        k.beam = CreateEffect(w, k.mx, k.my, k.mz);
        if (k.beam && !k.beam->dead && k.beam->hasEmitter)
            k.beam->attached = true;
    }
}
int main()
{
    World w; w.hasSubDef = true; w.known = "BEAM_ATTACK"; w.created = 0;
    for (int i = 0; i < 4; ++i) { w.pool[i].dead = false; w.pool[i].hasEmitter = true; }
    Kraken k; k.beam = 0; k.mx = 1; k.my = 2; k.mz = 3;
    StartBeamEffect(k, w);
    if (!k.beam || !k.beam->attached || k.beam->z != 3.0f) { std::printf("007fb6a0_TEST FAIL\n"); return 1; }
    w.pool[1].hasEmitter = false;
    StartBeamEffect(k, w);
    if (k.beam != &w.pool[1] || k.beam->attached) { std::printf("007fb6a0_TEST FAIL\n"); return 1; }
    w.known = "OTHER"; StartBeamEffect(k, w);
    if (w.created != 2) { std::printf("007fb6a0_TEST FAIL\n"); return 1; }
    w.known = "BEAM_ATTACK"; w.hasSubDef = false; StartBeamEffect(k, w);
    if (w.created != 2) { std::printf("007fb6a0_TEST FAIL\n"); return 1; }
    std::printf("007fb6a0_TEST PASS\n");
    return 0;
}