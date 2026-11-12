// Behaviour test for NCreatureMode::CModeDragonStrafeBurninate::StartBeamEffect (retail 0x008DC720): a copy of the model with
// recording callees. Checks: effect found -> frame stamped, effect created at the mouth position with the looked-up type, flame
// active, emitter attached to the owner's mouth dummy only for a live effect thing that has an emitter; effect missing -> nothing
// changes; the def reference counts balance in every case.
#include <cstdio>
#include <cstring>

struct Vec { float X, Y, Z; };
struct EffectsDef {
    long refCount; bool hasBurninate; long burninateType; int destroyed;
    bool GetEffectFromString(const char* name, long& type) const
    {
        if (hasBurninate && std::strcmp(name, "BURNINATE_ATTACK") == 0) { type = burninateType; return true; }
        return false;
    }
};
struct EffectsRef {
    EffectsDef* p;
    EffectsRef() : p(0) { }
    ~EffectsRef() { if (p && --p->refCount == 0) ++p->destroyed; }
};
struct BaseDef {
    long refCount; EffectsDef* sub; int destroyed;
    bool GetSubDef(EffectsRef* out) const { if (!sub) return false; out->p = sub; ++sub->refCount; return true; }
};
struct BaseDefRef {
    BaseDef* p;
    BaseDefRef(BaseDef* d) : p(d) { if (p) ++p->refCount; }
    ~BaseDefRef() { if (p && --p->refCount == 0) ++p->destroyed; }
};
struct Thing;
struct Emitter {
    Thing* attachedTo; long attachType; char helper[32]; long helperIndex; float offset; int attachCalls;
    void AttachToThing(Thing& thing, long type, const char* name, long index, float off)
    {
        attachedTo = &thing; attachType = type; std::strcpy(helper, name); helperIndex = index; offset = off; ++attachCalls;
    }
};
struct Thing {
    bool dead; Emitter* emitter; BaseDef* def; Vec mouth; int dummyCalls;
    bool GetDummyObject(const char* name, long, unsigned long, Vec& pos, float scale)
    {
        if (std::strcmp(name, "DRAGON_MOUTH_DUMMY") != 0 || scale != 1.0f) return false;
        pos = mouth; ++dummyCalls; return true;
    }
};
static long g_frame;
static Thing* g_createResult;
static long g_createType; static Vec g_createPos; static int g_createCalls;
static long GetFrame() { return g_frame; }
static Thing* Create(long type, const Vec& pos, bool)
{
    g_createType = type; g_createPos = pos; ++g_createCalls; return g_createResult;
}
struct Mode {
    Thing& ParentThing; Thing* PBeamWeaponEffect; long LastExplodeFrame; bool FlameActive;
    Mode(Thing& t) : ParentThing(t), PBeamWeaponEffect(0), LastExplodeFrame(-1), FlameActive(false) { }
    void StartBeamEffect()
    {
        EffectsRef effects;
        long effectType = 0;
        Vec position;
        ParentThing.GetDummyObject("DRAGON_MOUTH_DUMMY", 0, 0, position, 1.0f);
        if (BaseDefRef(ParentThing.def).p->GetSubDef(&effects) && effects.p->GetEffectFromString("BURNINATE_ATTACK", effectType)) {
            LastExplodeFrame = GetFrame();
            PBeamWeaponEffect = Create(effectType, position, false);
            FlameActive = true;
            if (PBeamWeaponEffect && !PBeamWeaponEffect->dead && PBeamWeaponEffect->emitter)
                PBeamWeaponEffect->emitter->AttachToThing(ParentThing, 0x2a, "DRAGON_MOUTH_DUMMY", 0, 0.0f);
        }
    }
};
static int Fail() { std::printf("008dc720_TEST FAIL\n"); return 1; }

int main()
{
    EffectsDef effects = { 1, true, 77, 0 };
    BaseDef def = { 1, &effects, 0 };
    Emitter emitter; std::memset(&emitter, 0, sizeof(emitter));
    Thing effectThing = { false, &emitter, 0, { 0, 0, 0 }, 0 };
    Thing owner = { false, 0, &def, { 1.5f, 2.5f, 3.5f }, 0 };

    // 1: effect found, live thing with an emitter -> everything happens once.
    g_frame = 1234; g_createResult = &effectThing; g_createCalls = 0;
    Mode a(owner);
    a.StartBeamEffect();
    if (owner.dummyCalls != 1 || g_createCalls != 1 || g_createType != 77) return Fail();
    if (g_createPos.X != 1.5f || g_createPos.Y != 2.5f || g_createPos.Z != 3.5f) return Fail();
    if (a.LastExplodeFrame != 1234 || !a.FlameActive || a.PBeamWeaponEffect != &effectThing) return Fail();
    if (emitter.attachCalls != 1 || emitter.attachedTo != &owner || emitter.attachType != 0x2a || emitter.helperIndex != 0
        || emitter.offset != 0.0f || std::strcmp(emitter.helper, "DRAGON_MOUTH_DUMMY") != 0) return Fail();
    if (def.refCount != 1 || effects.refCount != 1 || def.destroyed || effects.destroyed) return Fail();

    // 2: dead effect thing -> created and active, but not attached.
    effectThing.dead = true; g_frame = 2000;
    Mode b(owner);
    b.StartBeamEffect();
    if (g_createCalls != 2 || !b.FlameActive || b.LastExplodeFrame != 2000 || emitter.attachCalls != 1) return Fail();

    // 3: no emitter component / null thing -> not attached, still active.
    effectThing.dead = false; effectThing.emitter = 0;
    Mode c(owner);
    c.StartBeamEffect();
    if (g_createCalls != 3 || !c.FlameActive || emitter.attachCalls != 1) return Fail();
    g_createResult = 0;
    Mode d(owner);
    d.StartBeamEffect();
    if (g_createCalls != 4 || !d.FlameActive || d.PBeamWeaponEffect != 0 || emitter.attachCalls != 1) return Fail();

    // 4: effect name missing -> nothing changes; 5: no sub-def at all -> nothing changes.
    effects.hasBurninate = false; g_createResult = &effectThing;
    Mode e(owner);
    e.StartBeamEffect();
    if (g_createCalls != 4 || e.FlameActive || e.LastExplodeFrame != -1 || e.PBeamWeaponEffect != 0) return Fail();
    def.sub = 0;
    Mode f(owner);
    f.StartBeamEffect();
    if (g_createCalls != 4 || f.FlameActive || f.LastExplodeFrame != -1) return Fail();
    if (def.refCount != 1 || effects.refCount != 1 || def.destroyed || effects.destroyed || owner.dummyCalls != 6) return Fail();

    std::printf("008dc720_TEST PASS\n");
    return 0;
}