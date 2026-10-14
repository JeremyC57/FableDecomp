#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
struct CThing { int id; };
struct CCharString { const char* m_text; CCharString(const char* t, long = -1) : m_text(t) { } ~CCharString() { } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7;
static const char* g_anim; static int g_setCalls;
struct CCombatActionBase {
    virtual void v00() { }
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = 0; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; ++g_setCalls; }
};

struct CCreatureAction_DragonFlight : CCombatActionBase {
    unsigned char FaceTarget; float TurnRate;
    virtual void v00() { }
    CCreatureAction_DragonFlight(CThingCreatureBase* owner, const CCharString& animName) : CCombatActionBase(owner, 50, CSoundPair()), FaceTarget(0), TurnRate(0) { SetAnimationName(animName); }
};

int main()
{
    CThingCreatureBase owner; CThing target; CCharString name("ANIM_NAME");
    CCreatureAction_DragonFlight a(&owner, name);
    if (!(g_owner == &owner && g_code == 50 && g_s0 == 0 && g_s1 == 0 && a.FaceTarget == 0 && a.TurnRate == 0.0f && g_anim == name.m_text && g_setCalls == 1 && (char*)&a.TurnRate - (char*)&a == 0x118)) { std::printf("008f4cb0_TEST FAIL\n"); return 1; }
    std::printf("008f4cb0_TEST PASS\n");
    return 0;
}
