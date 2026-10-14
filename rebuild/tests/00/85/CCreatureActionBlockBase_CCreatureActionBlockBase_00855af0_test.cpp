#include <cstdio>
#include <cstring>
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

struct C3DVector { float X, Y, Z; C3DVector() { X = Y = Z = 0.0f; } };
struct CThingCreatureBase { int id; }; struct CThing { int id; };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0, g_s1;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); } };

struct CCreatureAction_BlockBase : CCombatActionBase { long BlowStrength; C3DVector MovementDirection; virtual void v00() {} CCreatureAction_BlockBase(CThingCreatureBase* owner, CThing* target, long actionCode, long blowStrength); };
CCreatureAction_BlockBase::CCreatureAction_BlockBase(CThingCreatureBase* owner, CThing* target, long actionCode, long blowStrength) : CCombatActionBase(owner, target, actionCode, CSoundPair()), BlowStrength(blowStrength), MovementDirection() { }
int main()
{
    CThingCreatureBase owner; CThing t; CCreatureAction_BlockBase a(&owner, &t, 77, 9);
    if (g_owner != &owner || g_target != &t || g_code != 77 || a.BlowStrength != 9 || a.MovementDirection.X != 0 || a.MovementDirection.Y != 0 || a.MovementDirection.Z != 0 || (char*)&a.MovementDirection - (char*)&a != 0x118 || sizeof(a) != 0x124) { std::printf("00855af0_TEST FAIL\n"); return 1; }
    std::printf("00855af0_TEST PASS\n");
    return 0;
}