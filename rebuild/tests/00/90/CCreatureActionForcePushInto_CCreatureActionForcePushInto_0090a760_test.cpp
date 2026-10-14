#include <cstdio>

// Model of CCreatureAction_ForcePushInto constructor @ 0x0090A760: base(owner, 150, zeroed CSoundPair) + members

struct CThingCreatureBase { int id; };
struct CSoundPair { long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
static CThingCreatureBase* g_owner = 0; static long g_code = -1; static long g_first = -1, g_second = -1; static int g_baseCalls = 0;
struct CCombatActionBase {
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s) { ++g_baseCalls; g_owner = owner; g_code = code; g_first = s.m_first; g_second = s.m_second; }
};

struct CCreatureAction_ForcePushInto : CCombatActionBase {
    bool m_pushLeft;
    virtual void v00() {}
    CCreatureAction_ForcePushInto(CThingCreatureBase* owner, bool pushLeft) : CCombatActionBase(owner, 150, CSoundPair()), m_pushLeft(pushLeft) { }
};
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CCreatureAction_ForcePushInto action(&owner, true);
    if (g_baseCalls != 1 || g_owner != &owner || g_code != 150 || g_first != 0 || g_second != 0 || action.m_pushLeft != true) { std::printf("0090a760_TEST FAIL\n"); return 1; }
    std::printf("0090a760_TEST PASS\n"); return 0;
}