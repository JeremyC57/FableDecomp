// Behaviour test for 0x008F7F50 NCombatActionHelpers::CreateEffect (vector overload): spawn at position, pass DIRECTION.
#include <cstdio>

class C3DVector { public: float X, Y, Z; };

struct Emitter { int updates; unsigned int key; C3DVector value; };
struct Thing { unsigned int presentBits[9]; Emitter* driver; C3DVector spawnedAt; };

static Thing   g_thing;
static Emitter g_stubEmitter;
static int     g_creates = 0;

static Thing* CreateEffectThing(long type, const C3DVector* position, bool)
{
    ++g_creates;
    if (type == 99) return 0;                       // creation failed
    g_thing.spawnedAt = *position;
    return &g_thing;
}
static unsigned int Calc(const char* s) { unsigned int h = 0; while (*s) h = h * 31 + (unsigned char)*s++; return h; }

static void CreateEffect(long effectType, const C3DVector& direction, const C3DVector& position)
{
    if (!effectType) return;
    Thing* effect = CreateEffectThing(effectType, &position, false);
    if (!effect) return;
    Emitter* emitter = 0;
    if (effect->presentBits[0x4b >> 5] & (1u << (0x4b & 31))) emitter = effect->driver;
    unsigned int key = Calc("DIRECTION");
    ++emitter->updates; emitter->key = key; emitter->value = direction;
}

static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    g_thing.presentBits[2] = 1u << 11; g_thing.driver = &g_stubEmitter; g_stubEmitter.updates = 0;
    C3DVector dir = { 0.0f, 1.0f, 0.0f }, pos = { 5.0f, 6.0f, 7.0f };
    int fail = 0;
    CreateEffect(0, dir, pos);
    if (g_creates != 0) fail = 1;                   // no effect type: nothing
    CreateEffect(99, dir, pos);
    if (g_creates != 1 || g_stubEmitter.updates != 0) fail = 1;
    CreateEffect(3, dir, pos);
    if (g_stubEmitter.updates != 1 || g_stubEmitter.key != Calc("DIRECTION") || !near(g_stubEmitter.value.Y, 1.0f)) fail = 1;
    if (!near(g_thing.spawnedAt.Z, 7.0f)) fail = 1;
    if (fail) { std::printf("008f7f50_TEST FAIL\n"); return 1; }
    std::printf("008f7f50_TEST PASS\n");
    return 0;
}