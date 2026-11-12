// Behaviour test for CCreatureAction_SpiritAttack::CreateAttackEffect (0x008548B0): a copy of the control flow over small
// models -- sub-def lookup gate, effect creation at the target, alive / emitter gates, attachment, and the two vector
// parameters (LIGHTNING_START = spirit focal point, LIGHTNING_END = target focal point) with Dirty set by the first insert.
#include <cstdio>
#include <cstring>
struct Vec { float X, Y, Z; };
struct Param { unsigned long key; Vec v; };
static unsigned long Crc(const char* s) { unsigned long h = 0; while (*s) h = h * 31 + (unsigned char)*s++; return h; }
struct Emitter { int attached; long attachType; Param params[4]; int count; bool dirty; int updates; };
struct Thing { Vec pos; Vec focal; bool dead; Emitter* emitter; };
struct Spirit { Vec focal; long effectIndex; };
struct World { bool hasSubDef; Thing created; long createdIndex; Vec createdAt; int creates; };
struct Action { Thing* target; Thing* creature; Spirit* spirit; Thing* particleEmitter; };

static void CreateAttackEffect(Action& a, World& w)
{
    Thing* target = a.target;
    if (!w.hasSubDef) return;
    const Spirit* spirit = a.spirit;
    long effectIndex = spirit->effectIndex;
    w.createdIndex = effectIndex; w.createdAt = target->pos; ++w.creates;
    a.particleEmitter = &w.created;
    if (a.particleEmitter && !a.particleEmitter->dead) {
        Emitter* e = a.particleEmitter->emitter;
        if (e) {
            e->attached = 1; e->attachType = 12;
            Param start = { Crc("LIGHTNING_START"), spirit->focal };
            Param end = { Crc("LIGHTNING_END"), target->focal };
            Param block[4]; int n = 0; bool dirty = false;
            block[n++] = start; dirty = true;    // Insert
            block[n++] = end;                    // push_back
            std::memcpy(e->params, block, sizeof(block)); e->count = n; e->dirty = dirty; ++e->updates;
        }
    }
}

int main()
{
    Emitter em = { 0, 0, {}, 0, false, 0 };
    Thing target = { {1, 2, 3}, {4, 5, 6}, false, 0 };
    Thing creature = { {0, 0, 0}, {0, 0, 0}, false, 0 };
    Spirit spirit = { {7, 8, 9}, 42 };
    World w = { true, { {0, 0, 0}, {0, 0, 0}, false, &em }, 0, {0, 0, 0}, 0 };
    Action a = { &target, &creature, &spirit, 0 };
    CreateAttackEffect(a, w);
    bool ok = w.creates == 1 && w.createdIndex == 42 && w.createdAt.Z == 3 && a.particleEmitter == &w.created &&
              em.attached == 1 && em.attachType == 12 && em.count == 2 && em.dirty &&
              em.params[0].key == Crc("LIGHTNING_START") && em.params[0].v.X == 7 &&
              em.params[1].key == Crc("LIGHTNING_END") && em.params[1].v.Z == 6;
    // no sub-def: nothing created
    World w2 = w; w2.hasSubDef = false; w2.creates = 0; Action a2 = { &target, &creature, &spirit, 0 };
    CreateAttackEffect(a2, w2);
    ok = ok && w2.creates == 0 && a2.particleEmitter == 0;
    // dead effect: created and kept, but no parameters
    Emitter em3 = { 0, 0, {}, 0, false, 0 };
    World w3 = { true, { {0, 0, 0}, {0, 0, 0}, true, &em3 }, 0, {0, 0, 0}, 0 };
    Action a3 = { &target, &creature, &spirit, 0 };
    CreateAttackEffect(a3, w3);
    ok = ok && w3.creates == 1 && a3.particleEmitter == &w3.created && em3.updates == 0;
    if (!ok) { std::printf("008548b0_TEST FAIL\n"); return 1; }
    std::printf("008548b0_TEST PASS\n");
    return 0;
}