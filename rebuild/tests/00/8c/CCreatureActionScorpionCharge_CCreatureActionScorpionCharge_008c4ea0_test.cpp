// Behaviour test for 0x008C4EA0 CCreatureAction_ScorpionCharge ctor: runs the reconstructed constructor over
// stubbed callees and checks the base arguments, the sound name, the animation name and both body assignments.
#include <cstdio>
#include <cstring>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CThing {
    C3DVector m_pos;
    const C3DVector& GetPosition() const { return m_pos; }
};
struct CThingCreatureBase : CThing { };

static char g_lastName[64];
static int g_live = 0;
struct CCharString {
    const char* m_pData;
    CCharString(const char* text, long = -1) : m_pData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
static char g_sound[64];
CSoundPair GetSoundPair(CThingCreatureBase*, const CCharString& name)
{
    std::strcpy(g_sound, name.m_pData);
    CSoundPair p; p.FirstSound = 11; p.LastSound = 22; return p;
}
struct CCombatActionBase {
    CThingCreatureBase* m_owner; CThing* m_target; long m_code; CSoundPair m_sound;
    virtual void v00() { }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long code, const CSoundPair& s)
        : m_owner(owner), m_target(target), m_code(code), m_sound(s) { }
    void SetAnimationName(const CCharString& name) { std::strcpy(g_lastName, name.m_pData); }
};
struct CCreatureAction_ScorpionCharge : CCombatActionBase {
    float     ChargeDistance;
    C3DVector StartPos;
    virtual void v00() { }
    CCreatureAction_ScorpionCharge(CThingCreatureBase* owner, CThing* target, float chargeDistance);
};
CCreatureAction_ScorpionCharge::CCreatureAction_ScorpionCharge(CThingCreatureBase* owner, CThing* target, float chargeDistance)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    {
        CCharString name("CHARGE_LOOP", -1);
        SetAnimationName(name);
    }
    ChargeDistance = chargeDistance;
    StartPos = ((CThing*)owner)->GetPosition();
}

int main()
{
    CThingCreatureBase owner; owner.m_pos.X = 1.5f; owner.m_pos.Y = -2.0f; owner.m_pos.Z = 7.25f;
    CThing target;
    CCreatureAction_ScorpionCharge a(&owner, &target, 12.5f);
    if (a.m_owner != &owner || a.m_target != &target || a.m_code != 150 || a.m_sound.FirstSound != 11 ||
        a.m_sound.LastSound != 22 || std::strcmp(g_sound, "SWORD_SWING") != 0 || std::strcmp(g_lastName, "CHARGE_LOOP") != 0 ||
        a.ChargeDistance != 12.5f || a.StartPos.X != 1.5f || a.StartPos.Y != -2.0f || a.StartPos.Z != 7.25f || g_live != 0)
    { std::printf("008c4ea0_TEST FAIL\n"); return 1; }
    std::printf("008c4ea0_TEST PASS\n");
    return 0;
}