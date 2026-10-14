#include <cstdio>
#include <cstring>
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct C3DVector { float X, Y, Z; };
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0, g_s1;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); } };

struct CCreatureAction_BalverineCorneredJumpIntoSky : CCombatActionBase { C3DVector* EndPos; virtual void v00() {} CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase* owner, CThing* target, C3DVector* endPos); };
CCreatureAction_BalverineCorneredJumpIntoSky::CCreatureAction_BalverineCorneredJumpIntoSky(CThingCreatureBase* owner, CThing* target, C3DVector* endPos) : CCombatActionBase(owner, target, 150, CSoundPair()), EndPos(endPos) { }
int main()
{
    CThingCreatureBase owner; CThing t; C3DVector d = { 1, 2, 3 }; CCreatureAction_BalverineCorneredJumpIntoSky a(&owner, &t, &d);
    if (g_owner != &owner || g_target != &t || g_code != 150 || a.EndPos != &d || (char*)&a.EndPos - (char*)&a != 0x114) { std::printf("008ccb80_TEST FAIL\n"); return 1; }
    std::printf("008ccb80_TEST PASS\n");
    return 0;
}