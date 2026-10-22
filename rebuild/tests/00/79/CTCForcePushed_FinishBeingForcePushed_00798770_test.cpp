// Behaviour test for CTCForcePushed::FinishBeingForcePushed (0x00798770): the model and body of final.cpp with the
// callees stubbed. Checks: (1) a game object owner takes a knock-down hit at its position along the push direction with
// the given damage when reversed (direction negated) and 1 point otherwise, with GuaranteeHit set and the dust effect off;
// (2) a non-game-object owner takes no hit; (3) the damage floor; (4) an AI creature owner has its animation resumed; (5) a
// live particle is killed, a dead one is not; (6) the component removes itself (0x8d) every time.
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif

class CThing;
class CBaseObject;
namespace NHitLocations { class CHitLocation; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
};

static int g_hitCtors = 0, g_hitDtors = 0, g_damageCalls = 0;
static CThing* g_hitThingHitting = 0; static CThing* g_hitWeapon = 0; static void* g_hitLocation = (void*)1;
static C3DVector g_hitPos(0, 0, 0), g_hitDir(0, 0, 0);
static bool g_hitKnockDown = false, g_hitBlockable = true, g_hitFlourish = true, g_hitCounter = true;
static long g_damageAttribute = -1; static float g_damage = -1.0f;

class CHitParameters {
public:
    unsigned char _pad00[0x22];
    bool MoveBack;
    bool CreateParticleEffectOnHit;
    bool CreateDustParticleEffectOnHit;
    bool GuaranteeHit;
    unsigned char _pad26[0x60 - 0x26];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter)
    {
        g_hitCtors++; g_hitLocation = hitLocation; g_hitThingHitting = thingHitting; g_hitWeapon = weapon;
        g_hitPos = position; g_hitDir = direction; g_hitKnockDown = knockDown; g_hitBlockable = blockable;
        g_hitFlourish = flourish; g_hitCounter = blockCounter;
        CreateDustParticleEffectOnHit = true; GuaranteeHit = false; MoveBack = false; CreateParticleEffectOnHit = false;
    }
    ~CHitParameters() { g_hitDtors++; }
    void AddDamageWithAttribute(long attribute, float damage) { g_damageCalls++; g_damageAttribute = attribute; g_damage = damage; }
};

struct CBaseIntelligentPointer {
    virtual void v00() {}
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};

static int g_onHitCalls = 0; static bool g_onHitGuarantee = false, g_onHitDust = true;
static int g_killCalls = 0, g_removeCalls = 0; static long g_removeType = 0; static bool g_killImmediate = false;
static int g_suspendCalls = 0; static bool g_suspend = true; static long g_suspendReason = -1;
class CThing {
public:
    virtual void v000() {}
    virtual void OnHit(CHitParameters& hit) { g_onHitCalls++; g_onHitGuarantee = hit.GuaranteeHit; g_onHitDust = hit.CreateDustParticleEffectOnHit; }
    unsigned char ThingClass;
    unsigned long ThingTypeFlags;
    unsigned char m_flags91;
    C3DVector Position;
    const C3DVector& GetPosition() const { return Position; }
    void Kill(bool immediately) { g_killCalls++; g_killImmediate = immediately; }
    void RemoveTC(long interfaceType) { g_removeCalls++; g_removeType = interfaceType; }
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason) { g_suspendCalls++; g_suspend = suspend; g_suspendReason = reason; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CTCForcePushed {
public:
    virtual void v00() {}
    CThing* ParentThing;
    CBaseIntelligentPointer PThingCausingForcePush;
    CBaseIntelligentPointer PParticle;
    C3DVector PositionForcePushStarted;
    float DistanceBeingPushed;
    C3DVector DirectionBeingPushed;
    float DistanceMovingPerFrame;
    bool Initialised;
    bool StartedMoving;
    void FinishBeingForcePushed(bool reversed, float damage);
    __forceinline void HitOwner(CThing* owner, C3DVector position, const C3DVector& direction, float damage)
    {
        CHitParameters hit(0, PThingCausingForcePush.GetPItem(), 0, position, direction, true, false, false, false);
        hit.GuaranteeHit = true;
        hit.AddDamageWithAttribute(8, damage);
        hit.CreateDustParticleEffectOnHit = false;
        owner->OnHit(hit);
    }
};

void CTCForcePushed::FinishBeingForcePushed(bool reversed, float damage)
{
    if (damage < 0.0001f)
        damage = 0.001f;
    CThing* owner = ParentThing;
    if (owner->ThingTypeFlags & 8) {
        C3DVector direction(DirectionBeingPushed.X, DirectionBeingPushed.Y, DirectionBeingPushed.Z);
        if (reversed) {
            direction = -direction;
            HitOwner(owner, owner->GetPosition(), direction, damage);
        } else
            HitOwner(owner, owner->GetPosition(), direction, 1.0f);
    }
    if (ParentThing->ThingClass == 1)
        ((CThingAnimatable*)ParentThing)->SetSuspendAnimation(false, 0);
    if (IS_THING_ALIVE(PParticle))
        PParticle.GetPItem()->Kill(true);
    ParentThing->RemoveTC(0x8d);
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CThingAnimatable owner; owner.ThingClass = 1; owner.ThingTypeFlags = 8; owner.m_flags91 = 0;
    owner.Position = C3DVector(10.0f, 20.0f, 30.0f);
    CThing pusher; CThing particle; particle.m_flags91 = 0;
    CTCForcePushed tc;
    tc.ParentThing = &owner; tc.PThingCausingForcePush.m_pItem = &pusher; tc.PParticle.m_pItem = &particle;
    tc.DirectionBeingPushed = C3DVector(1.0f, -2.0f, 0.5f);

    // (1) reversed hit with damage 7: negated direction, owner position, GuaranteeHit, dust off, knock-down flags
    tc.FinishBeingForcePushed(true, 7.0f);
    CHECK(g_hitCtors == 1 && g_hitDtors == 1 && g_onHitCalls == 1);
    CHECK(g_hitLocation == 0 && g_hitThingHitting == &pusher && g_hitWeapon == 0);
    CHECK(Near(g_hitPos.X, 10.0f) && Near(g_hitPos.Y, 20.0f) && Near(g_hitPos.Z, 30.0f));
    CHECK(Near(g_hitDir.X, -1.0f) && Near(g_hitDir.Y, 2.0f) && Near(g_hitDir.Z, -0.5f));
    CHECK(g_hitKnockDown && !g_hitBlockable && !g_hitFlourish && !g_hitCounter);
    CHECK(g_damageCalls == 1 && g_damageAttribute == 8 && Near(g_damage, 7.0f));
    CHECK(g_onHitGuarantee && !g_onHitDust);
    // (4) AI creature owner: animation resumed; (5) live particle killed; (6) component removed
    CHECK(g_suspendCalls == 1 && !g_suspend && g_suspendReason == 0);
    CHECK(g_killCalls == 1 && g_killImmediate);
    CHECK(g_removeCalls == 1 && g_removeType == 0x8d);
    CHECK(Near(tc.DirectionBeingPushed.X, 1.0f) && Near(tc.DirectionBeingPushed.Y, -2.0f));   // member untouched

    // (1b) not reversed: plain direction and 1 point of damage; (3) the damage floor applies before the test
    tc.FinishBeingForcePushed(false, 0.0f);
    CHECK(g_hitCtors == 2 && g_onHitCalls == 2 && Near(g_damage, 1.0f));
    CHECK(Near(g_hitDir.X, 1.0f) && Near(g_hitDir.Y, -2.0f) && Near(g_hitDir.Z, 0.5f));

    // (3) reversed with a tiny damage: floored to 0.001
    tc.FinishBeingForcePushed(true, 0.00001f);
    CHECK(g_hitCtors == 3 && Near(g_damage, 0.001f));

    // (2) owner is not a game object (bit 3 clear), not an AI creature, particle dead: only the removal happens
    owner.ThingTypeFlags = 0; owner.ThingClass = 0; particle.m_flags91 = 1;
    tc.FinishBeingForcePushed(true, 5.0f);
    CHECK(g_hitCtors == 3 && g_onHitCalls == 3 && g_suspendCalls == 3 && g_killCalls == 3 && g_removeCalls == 4);

    // no particle at all
    tc.PParticle.m_pItem = 0;
    tc.FinishBeingForcePushed(false, 5.0f);
    CHECK(g_killCalls == 3 && g_removeCalls == 5);

    char line[64];
    sprintf(line, "00798770_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}