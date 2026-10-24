// Behaviour test for 0x008F7E70 NCombatActionHelpers::CreateEffect (helper overload): spawn at the creature, attach if named.
#include <cstdio>

struct C3DVector { float X, Y, Z; };

struct Thing;
struct Emitter { int updates, attaches; long attachType; const char* helper; Thing* attachedTo; };
struct Thing { unsigned int presentBits[9]; Emitter* driver; C3DVector position; };

static Thing   g_effect;
static Emitter g_stubEmitter;

static Thing* CreateEffectThing(long, const C3DVector* position, bool) { g_effect.position = *position; return &g_effect; }

static void CreateEffect(Thing& creature, long effectType, const C3DVector& direction, const char* const* helperName)
{
    if (!effectType) return;
    Thing* effect = CreateEffectThing(effectType, &creature.position, false);
    if (!effect) return;
    Emitter* emitter = 0;
    if (effect->presentBits[0x4b >> 5] & (1u << (0x4b & 31))) emitter = effect->driver;
    if (helperName) {
        ++emitter->attaches; emitter->attachedTo = &creature; emitter->attachType = 12; emitter->helper = *helperName;
    }
    (void)direction;
    ++emitter->updates;
}

int main()
{
    g_effect.presentBits[2] = 1u << 11; g_effect.driver = &g_stubEmitter;
    g_stubEmitter.updates = g_stubEmitter.attaches = 0;
    Thing hero; hero.position.X = 1.0f; hero.position.Y = 2.0f; hero.position.Z = 3.0f;
    C3DVector dir = { 1.0f, 0.0f, 0.0f };
    const char* helper = "weapon_pos_a";
    int fail = 0;
    CreateEffect(hero, 4, dir, 0);
    if (g_stubEmitter.updates != 1 || g_stubEmitter.attaches != 0) fail = 1;
    CreateEffect(hero, 4, dir, &helper);
    if (g_stubEmitter.updates != 2 || g_stubEmitter.attaches != 1 || g_stubEmitter.attachType != 12 || g_stubEmitter.attachedTo != &hero) fail = 1;
    float d = g_effect.position.Z - 3.0f;
    if (d > 0.001f || d < -0.001f) fail = 1;
    if (fail) { std::printf("008f7e70_TEST FAIL\n"); return 1; }
    std::printf("008f7e70_TEST PASS\n");
    return 0;
}