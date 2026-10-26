// 007c25a0 behaviour test: CTCShotDefinitionBase::InitAttachedEffects (final.cpp body over mock things)
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C3DVector { public: float X, Y, Z; };
struct SVectorParam { unsigned long m_key; C3DVector m_value; SVectorParam(unsigned long k, const C3DVector& v) : m_key(k), m_value(v) {} };
struct CParticleUpdateParameters { std::vector<SVectorParam> Vector; };
class CThing;
class CTCDParticleEmitter { public:
    CThing* AttachedTo; long AttachType; std::string Helper; CParticleUpdateParameters Params;
    void AttachToThing(CThing* t, long type, const std::string& h, int, int) { AttachedTo = t; AttachType = type; Helper = h; }
    void SetEmitterUpdateParameters(const CParticleUpdateParameters& p) { Params = p; }
};
struct CThingAppearance {
    bool Dummy, Helper;
    bool HasDummyObject(const std::string&) { return Dummy; }
    bool HasHelperPoint(const char*) { return Helper; }
};
struct CPhysics { C3DVector Dir; C3DVector GetDirection() { return Dir; } };
class CThing { public: unsigned char m_flags91; CTCDParticleEmitter* Driver; CThingAppearance* Appearance; CPhysics* PhysicsTC; C3DVector Pos;
    bool HasTC(long) { return Driver != 0; } };
static CThing* g_created; static long g_createdType;
static CThing* CreateEffectThing(long type, const C3DVector*, bool) { g_createdType = type; return g_created; }
static unsigned long Crc(const char*) { return 0x1234; }
class CThingShotDef { public: long AttachedContinuousEffect; bool UseEndTimeLineOnContinousEffect; };
struct CTCShotDefinitionBase {
    CThing* ParentThing; CThing* PEffect; const CThingShotDef* PShotDef;
    void InitAttachedEffects();
};
void CTCShotDefinitionBase::InitAttachedEffects()
{
    long effectType = PShotDef->AttachedContinuousEffect;
    if (!effectType)
        return;
    CThing* effect = CreateEffectThing(effectType, &ParentThing->Pos, false);
    if (!effect || (effect->m_flags91 & 1))
        return;
    PEffect = effect;
    CTCDParticleEmitter* emitter = 0;
    if (effect->HasTC(0x4b))
        emitter = effect->Driver;
    std::string helperName;
    CThingAppearance* appearance = ParentThing->Appearance;
    long attachType;
    if (appearance->HasDummyObject("SHOT_CREATION_DUMMY")) {
        helperName = "SHOT_CREATION_DUMMY";
        attachType = 10;
    } else if (appearance->HasHelperPoint("weapon_pos_a")) {
        helperName = "weapon_pos_a";
        attachType = 12;
    } else
        attachType = 0x28;
    if (PShotDef->UseEndTimeLineOnContinousEffect)
        attachType |= 0x10;
    emitter->AttachToThing(ParentThing, attachType, helperName, 0, 0);
    unsigned long key = Crc("MOVE_DIRECTION");
    SVectorParam moveDirection(key, ParentThing->PhysicsTC->GetDirection());
    CParticleUpdateParameters parameters;
    parameters.Vector.push_back(moveDirection);
    emitter->SetEmitterUpdateParameters(parameters);
}
int main()
{
    CTCDParticleEmitter em; em.AttachedTo = 0; em.AttachType = -1;
    CThing effect = { 0, &em, 0, 0, { 0, 0, 0 } };
    CThingAppearance app = { false, true }; CPhysics ph = { { 1.0f, 2.0f, 3.0f } };
    CThing shooter = { 0, 0, &app, &ph, { 5, 6, 7 } };
    CThingShotDef def = { 77, true };
    CTCShotDefinitionBase s = { &shooter, 0, &def };
    g_created = &effect;
    s.InitAttachedEffects();
    CHECK(g_createdType == 77 && s.PEffect == &effect);
    CHECK(em.AttachedTo == &shooter && em.AttachType == (12 | 0x10) && em.Helper == "weapon_pos_a");
    CHECK(em.Params.Vector.size() == 1 && em.Params.Vector[0].m_key == 0x1234 && em.Params.Vector[0].m_value.Z == 3.0f);
    app.Dummy = true; def.UseEndTimeLineOnContinousEffect = false; s.InitAttachedEffects();
    CHECK(em.AttachType == 10 && em.Helper == "SHOT_CREATION_DUMMY");
    app.Dummy = false; app.Helper = false; s.InitAttachedEffects();
    CHECK(em.AttachType == 0x28 && em.Helper.empty());
    effect.m_flags91 = 1; s.PEffect = 0; em.AttachType = -1; s.InitAttachedEffects();
    CHECK(s.PEffect == 0 && em.AttachType == -1);
    def.AttachedContinuousEffect = 0; g_createdType = 0; s.InitAttachedEffects();
    CHECK(g_createdType == 0);
    if (!g_fail) printf("007c25a0_TEST PASS\n");
    return g_fail;
}