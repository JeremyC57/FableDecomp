#include <cstdio>
#include <cstring>
// Model of CCreatureAction_TrollIdle constructor @ 0x00908270: base(owner, target, 50, zeroed CSoundPair) then SetAnimationName("SPECIAL_IDLE")
struct CThingCreatureBase { int id; }; struct CThing { int id; };
struct CSoundPair { long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
static int g_strCtor = 0, g_strDtor = 0;
struct CCharString { const char* m_text; CCharString(const char* t, long) : m_text(t) { ++g_strCtor; } ~CCharString() { ++g_strDtor; } };
static CThingCreatureBase* g_owner = 0; static CThing* g_target = 0; static long g_code = -1; static long g_first = -1, g_second = -1; static int g_baseCalls = 0;
static const char* g_animName = 0; static int g_setCallsAfterBase = -1;
struct CCombatActionBase {
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long code, const CSoundPair& s) { ++g_baseCalls; g_owner = owner; g_target = target; g_code = code; g_first = s.m_first; g_second = s.m_second; }
    void SetAnimationName(const CCharString& name) { g_animName = name.m_text; g_setCallsAfterBase = g_baseCalls; }
};
struct CCreatureAction_TrollIdle : CCombatActionBase {
    virtual void v00() {}
    CCreatureAction_TrollIdle(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, target, 50, CSoundPair()) { CCharString name("SPECIAL_IDLE", -1); SetAnimationName(name); }
};
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_TrollIdle action(&owner, &target);
    bool ok = g_baseCalls == 1 && g_owner == &owner && g_target == &target && g_code == 50 && g_first == 0 && g_second == 0
        && g_animName != 0 && std::strcmp(g_animName, "SPECIAL_IDLE") == 0 && g_setCallsAfterBase == 1 && g_strCtor == 1 && g_strDtor == 1;
    if (!ok) { std::printf("00908270_TEST FAIL\n"); return 1; }
    std::printf("00908270_TEST PASS\n"); return 0;
}