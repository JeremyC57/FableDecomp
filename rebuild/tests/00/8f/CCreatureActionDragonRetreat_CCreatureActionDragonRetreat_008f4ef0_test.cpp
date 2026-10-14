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

struct CCreatureAction_DragonRetreat : CCombatActionBase {
    virtual void v00() { }
    CCreatureAction_DragonRetreat(CThingCreatureBase* owner, const CCharString& animName) : CCombatActionBase(owner, 250, CSoundPair()) { SetAnimationName(animName); }
};

int main()
{
    CThingCreatureBase owner; CThing target; CCharString name("ANIM_NAME");
    CCreatureAction_DragonRetreat a(&owner, name);
    if (!(g_owner == &owner && g_code == 250 && g_s0 == 0 && g_s1 == 0 && g_anim == name.m_text && g_setCalls == 1 && sizeof(a) == 0x114)) { std::printf("008f4ef0_TEST FAIL\n"); return 1; }
    std::printf("008f4ef0_TEST PASS\n");
    return 0;
}