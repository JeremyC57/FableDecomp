// Behaviour test for CThingObject::Create (0x00703210): host model of the allocate / construct / place physics / activate
// sequence, with stubs for the callees. Prints 00703210_TEST PASS.
#include <stdio.h>

struct Vec { float X, Y, Z; };
struct RHSet { Vec Up, Forward; };

static int g_allocOk, g_constructOk, g_activateOk;
static int g_constructCalls, g_physicsCalls, g_activateCalls;
static RHSet g_lastSet;
static Vec g_lastPos;

struct Physics {
    void ConstructFromParams(const Vec& p, const RHSet& r) { ++g_physicsCalls; g_lastPos = p; g_lastSet = r; }
};
struct Object {
    Physics phys;
    Physics* PhysicsTC;
    Object() : PhysicsTC(&phys) {}
    bool ConstructFromParams(long, const Vec&, long, unsigned long, const char*) { ++g_constructCalls; return g_constructOk != 0; }
    bool DoActivationSequence() { ++g_activateCalls; return g_activateOk != 0; }
};
static Object g_object;
static Object* Alloc() { return g_allocOk ? &g_object : 0; }

// the row's logic
static Object* Create(long defIndex, const Vec& position, long player, const RHSet* rhSet, unsigned long flags, const char* name)
{
    Object* object = Alloc();
    Object* result = 0;
    if (object && object->ConstructFromParams(defIndex, position, player, flags, name)) {
        if (rhSet) {
            object->PhysicsTC->ConstructFromParams(position, *rhSet);
        } else {
            RHSet identity;
            identity.Up.X = 0.0f; identity.Up.Y = 0.0f; identity.Up.Z = 1.0f;
            identity.Forward.X = 0.0f; identity.Forward.Y = 1.0f; identity.Forward.Z = 0.0f;
            object->PhysicsTC->ConstructFromParams(position, identity);
        }
        if (object->DoActivationSequence())
            result = object;
    }
    return result;
}

int main()
{
    int fails = 0;
    Vec pos = { 1.0f, 2.0f, 3.0f };
    RHSet set = { { 0.5f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.5f } };
    g_allocOk = 0;
    if (Create(1, pos, 0, 0, 0, "a") != 0 || g_constructCalls != 0) ++fails;
    g_allocOk = 1; g_constructOk = 0;
    if (Create(1, pos, 0, 0, 0, "a") != 0 || g_constructCalls != 1 || g_physicsCalls != 0) ++fails;
    g_constructOk = 1; g_activateOk = 1;
    if (Create(1, pos, 0, 0, 0, "a") != &g_object || g_physicsCalls != 1) ++fails;
    if (g_lastSet.Up.Z != 1.0f || g_lastSet.Forward.Y != 1.0f || g_lastSet.Up.X != 0.0f || g_lastPos.Y != 2.0f) ++fails;
    if (Create(1, pos, 0, &set, 0, "a") != &g_object || g_lastSet.Up.X != 0.5f || g_lastSet.Forward.Z != 0.5f) ++fails;
    g_activateOk = 0;
    if (Create(1, pos, 0, &set, 0, "a") != 0 || g_activateCalls != 3) ++fails;
    if (fails == 0) printf("00703210_TEST PASS\n");
    else printf("00703210_TEST FAIL %d\n", fails);
    return fails;
}