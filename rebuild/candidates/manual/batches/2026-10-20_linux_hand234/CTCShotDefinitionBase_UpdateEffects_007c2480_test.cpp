// Behaviour test for 0x007C2480 CTCShotDefinitionBase::UpdateEffects (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
static unsigned long Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{
    const unsigned char* p = (const unsigned char*)data; unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + p[i];
    return h;
}
static const float g_epsilon_0129ba3c = 1e-4f;
struct CBaseClass { virtual ~CBaseClass() {} };
struct SVectorParam {
    unsigned long m_key; C3DVector m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
static int g_pushes = 0; static unsigned long g_lastKey = 0; static C3DVector g_lastValue;
struct CVectorParamVector {
    void* a; void* b; void* c;
    CVectorParamVector() : a(0), b(0), c(0) {}
    void push_back(const SVectorParam& v) { ++g_pushes; g_lastKey = v.m_key; g_lastValue = v.m_value; }
};
struct CVectorParamMap { CVectorParamVector Vector; bool Dirty; CVectorParamMap() : Dirty(false) {} };
struct CParticleUpdateParameters : CBaseClass {
    CVectorParamMap VectorParams;
    CParticleUpdateParameters() {}
};
static const CParticleUpdateParameters* g_setWith = 0; static int g_sets = 0;
struct CTCDParticleEmitter {
    void SetEmitterUpdateParameters(const CParticleUpdateParameters& p) { g_setWith = &p; ++g_sets; }
};
struct CTCPhysicsBase { C3DVector Position; C3DVector OldPosition; };
class CThing {
public:
    unsigned long   m_presentBits[9];
    void*           PTCDriver;
    CTCPhysicsBase* PhysicsTC;
    unsigned char   m_flags91;
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
};
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
static bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_flags91 & 1)) return true; }
    return false;
}
struct CTCShotDefinitionBase {
    CThing* ParentThing;
    CBaseIntelligentPointer PEffect;
    void UpdateEffects();
};
void CTCShotDefinitionBase::UpdateEffects()
{
    if (!IS_THING_ALIVE(PEffect))
        return;
    CTCDParticleEmitter* emitter = 0;
    CThing* effect = PEffect.GetPItem();
    if (effect->HasTC(0x4b))
        emitter = (CTCDParticleEmitter*)effect->PTCDriver;
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    C3DVector movement(physics->Position.X - physics->OldPosition.X, physics->Position.Y - physics->OldPosition.Y,
                       physics->Position.Z - physics->OldPosition.Z);
    float lengthSquared = movement.Z * movement.Z;
    lengthSquared += movement.Y * movement.Y;
    lengthSquared += movement.X * movement.X;
    if (!(lengthSquared > g_epsilon_0129ba3c * g_epsilon_0129ba3c)) {
        unsigned long key = Calc_004014a0(0, "MOVE_DIRECTION", 14);
        SVectorParam moveDirection(key, movement);
        CParticleUpdateParameters parameters;
        parameters.VectorParams.Vector.push_back(moveDirection);
        emitter->SetEmitterUpdateParameters(parameters);
    }
}
static int fail() { std::printf("007c2480_TEST FAIL\n"); return 1; }
int main()
{
    CTCDParticleEmitter emitter;
    CThing effect; std::memset(&effect, 0, sizeof effect);
    effect.m_presentBits[0x4b >> 5] = 1UL << (0x4b & 31); effect.PTCDriver = &emitter;
    CTCPhysicsBase phys; phys.Position = C3DVector(1.0f, 2.0f, 3.0f); phys.OldPosition = C3DVector(1.0f, 2.0f, 3.0f);
    CThing shot; std::memset(&shot, 0, sizeof shot); shot.PhysicsTC = &phys;
    CTCShotDefinitionBase def; def.ParentThing = &shot; def.PEffect.m_pItem = 0;
    def.UpdateEffects();                                    // no effect: nothing
    if (g_sets != 0) return fail();
    def.PEffect.m_pItem = &effect;
    def.UpdateEffects();                                    // still (zero movement): parameters sent
    if (g_sets != 1 || g_pushes != 1 || g_lastKey != Calc_004014a0(0, "MOVE_DIRECTION", 14) || g_lastValue.X != 0.0f) return fail();
    phys.Position = C3DVector(1.5f, 2.0f, 3.0f);            // moving 0.5: |d|^2 > 1e-8, skipped
    def.UpdateEffects();
    if (g_sets != 1) return fail();
    phys.Position = C3DVector(1.0f + 1e-5f, 2.0f, 3.0f);    // tiny move (|d| ~ 1e-5): body runs with that movement
    def.UpdateEffects();
    if (g_sets != 2 || !(g_lastValue.X > 0.0f)) return fail();
    effect.m_flags91 = 1;                                   // dead effect
    def.UpdateEffects();
    if (g_sets != 2) return fail();
    std::printf("007c2480_TEST PASS\n");
    return 0;
}
