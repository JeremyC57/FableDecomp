#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; };
static CThingCreatureBase* g_owner; static long g_a, g_b, g_s0, g_s1;
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

struct CCreatureActionBase { virtual void v00() {} unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, const CSoundPair& sound, long a, long b) { g_owner = owner; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; g_a = a; g_b = b; std::memset(_body, 0, sizeof(_body)); } };
struct CCreatureAction_ActivateZTarget : CCreatureActionBase { virtual void v00() {} CCreatureAction_ActivateZTarget(CThingCreatureBase* owner); };
CCreatureAction_ActivateZTarget::CCreatureAction_ActivateZTarget(CThingCreatureBase* owner) : CCreatureActionBase(owner, CSoundPair(), 100, 250) { }
int main()
{
    CThingCreatureBase owner; CCreatureAction_ActivateZTarget x(&owner);
    if (g_owner != &owner || g_a != 100 || g_b != 250 || g_s0 != 0 || g_s1 != 0 || sizeof(x) != 0xa8) { std::printf("00842460_TEST FAIL\n"); return 1; }
    std::printf("00842460_TEST PASS\n");
    return 0;
}