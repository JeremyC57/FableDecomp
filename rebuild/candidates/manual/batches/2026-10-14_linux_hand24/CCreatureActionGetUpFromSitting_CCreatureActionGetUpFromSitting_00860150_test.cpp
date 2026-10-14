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

struct CCreatureAction_GetUpFromSitting : CCombatActionBase {
    long SittingMode; unsigned char RemoveOnPerformAction;
    virtual void v00() { }
    CCreatureAction_GetUpFromSitting(CThingCreatureBase* owner, const CCharString& animName, long sittingMode, bool removeOnPerformAction) : CCombatActionBase(owner, 50, CSoundPair()), SittingMode(sittingMode), RemoveOnPerformAction(removeOnPerformAction) { SetAnimationName(animName); }
};

int main()
{
    CThingCreatureBase owner; CThing target; CCharString name("ANIM_NAME");
    CCreatureAction_GetUpFromSitting a(&owner, name, 3, true);
    if (!(g_owner == &owner && g_code == 50 && g_s0 == 0 && g_s1 == 0 && a.SittingMode == 3 && a.RemoveOnPerformAction == 1 && g_anim == name.m_text && g_setCalls == 1 && (char*)&a.RemoveOnPerformAction - (char*)&a == 0x118)) { std::printf("00860150_TEST FAIL\n"); return 1; }
    std::printf("00860150_TEST PASS\n");
    return 0;
}
