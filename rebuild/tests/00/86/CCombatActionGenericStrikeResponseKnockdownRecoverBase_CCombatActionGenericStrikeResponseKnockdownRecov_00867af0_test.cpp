#include <cstdio>

// Model of CCombatAction_GenericStrikeResponseKnockdownRecoverBase constructor @ 0x00867AF0: base(owner, 250, zeroed CSoundPair) + members

struct CThingCreatureBase { int id; };
struct CSoundPair { long m_first; long m_second; CSoundPair() : m_first(0), m_second(0) { } };
static CThingCreatureBase* g_owner = 0; static long g_code = -1; static long g_first = -1, g_second = -1; static int g_baseCalls = 0;
struct CCombatActionBase {
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long code, const CSoundPair& s) { ++g_baseCalls; g_owner = owner; g_code = code; g_first = s.m_first; g_second = s.m_second; }
};

struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase : CCombatActionBase {
    bool CanBeHit; bool Flip;
    virtual void v00() {}
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase(CThingCreatureBase* owner, bool flip) : CCombatActionBase(owner, 250, CSoundPair()), CanBeHit(false), Flip(flip) { }
};
int main()
{
    CThingCreatureBase owner; owner.id = 7;
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase action(&owner, true);
    if (g_baseCalls != 1 || g_owner != &owner || g_code != 250 || g_first != 0 || g_second != 0 || action.CanBeHit != false || action.Flip != true) { std::printf("00867af0_TEST FAIL\n"); return 1; }
    std::printf("00867af0_TEST PASS\n"); return 0;
}