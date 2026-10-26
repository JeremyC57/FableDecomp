#include <cstdio>
#include <cstring>
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CThingCreatureBase { public: int id; }; class CThing { public: int id; };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0, g_s1;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); } };

class CCreatureAction_Ambush : public CCombatActionBase { public: C3DVector JumpDest; virtual void v00() {} CCreatureAction_Ambush(CThingCreatureBase* owner, CThing* target, const C3DVector& d); };
CCreatureAction_Ambush::CCreatureAction_Ambush(CThingCreatureBase* owner, CThing* target, const C3DVector& d) : CCombatActionBase(owner, target, 250, CSoundPair()) , JumpDest(d) { }
int main()
{
    CThingCreatureBase owner; CThing t; C3DVector d(4.0f, 5.0f, -6.0f); CCreatureAction_Ambush a(&owner, &t, d);
    if (g_owner != &owner || g_target != &t || g_code != 250 || a.JumpDest.X != 4.0f || a.JumpDest.Y != 5.0f || a.JumpDest.Z != -6.0f || (char*)&a.JumpDest - (char*)&a != 0x114) { std::printf("008bfda0_TEST FAIL\n"); return 1; }
    std::printf("008bfda0_TEST PASS\n");
    return 0;
}