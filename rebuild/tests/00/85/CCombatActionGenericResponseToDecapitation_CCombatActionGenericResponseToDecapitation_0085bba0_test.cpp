#include <cstdio>

// Model of CCombatAction_GenericResponseToDecapitation constructor @ 0x0085BBA0: base(owner, 0, zeroed CSoundPair)

struct CThingCreatureBase { int id; };
struct CSoundPair { long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
static CThingCreatureBase* g_owner = 0; static long g_code = -1; static long g_first = -1, g_second = -1; static int g_baseCalls = 0;
struct CCombatActionBase {
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s) { ++g_baseCalls; g_owner = owner; g_code = code; g_first = s.m_first; g_second = s.m_second; }
};

struct CCombatAction_GenericResponseToDecapitation : CCombatActionBase {
    virtual void v00() {}
    CCombatAction_GenericResponseToDecapitation(CThingCreatureBase* owner) : CCombatActionBase(owner, 0, CSoundPair()) { }
};
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CCombatAction_GenericResponseToDecapitation action(&owner);
    if (g_baseCalls != 1 || g_owner != &owner || g_code != 0 || g_first != 0 || g_second != 0) { std::printf("0085bba0_TEST FAIL\n"); return 1; }
    std::printf("0085bba0_TEST PASS\n"); return 0;
}