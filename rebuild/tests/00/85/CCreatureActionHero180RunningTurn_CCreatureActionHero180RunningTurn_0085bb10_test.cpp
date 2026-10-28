#include <cstdio>
#include <cstring>
class CCharString { public: char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class C3DVector { public: float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { } };
class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const char* g_anim;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetCombatAnimation(const CCharString& name) { g_anim = name.m_pData; } };

class CCreatureActionHero180RunningTurn : public CCombatActionBase { public: C3DVector RequiredFacing; virtual void v00() {} CCreatureActionHero180RunningTurn(CThingCreatureBase* owner, const C3DVector& f); };
CCreatureActionHero180RunningTurn::CCreatureActionHero180RunningTurn(CThingCreatureBase* owner, const C3DVector& f) : CCombatActionBase(owner, 1, CSoundPair()) , RequiredFacing(f) { }
int main()
{
    CThingCreatureBase owner; C3DVector f(1.0f, -2.5f, 3.0f); CCreatureActionHero180RunningTurn a(&owner, f);
    if (g_owner != &owner || g_code != 1 || a.RequiredFacing.X != 1.0f || a.RequiredFacing.Y != -2.5f || a.RequiredFacing.Z != 3.0f || (char*)&a.RequiredFacing - (char*)&a != 0x114) { std::printf("0085bb10_TEST FAIL\n"); return 1; }
    std::printf("0085bb10_TEST PASS\n");
    return 0;
}