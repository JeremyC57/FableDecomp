#include <cstdio>
#include <cstring>
class CCharString { public: char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const char* g_anim;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_pData; } };

class CCreatureAction_BeetleLand : public CCombatActionBase { public: virtual void v00() {} CCreatureAction_BeetleLand(CThingCreatureBase* owner); };
CCreatureAction_BeetleLand::CCreatureAction_BeetleLand(CThingCreatureBase* owner) : CCombatActionBase(owner, 0, CSoundPair()) {
    {
        CCharString name("LAND");         // conversion-constructor length default (-1), re-addressed with lea
        SetAnimationName(name);            // 0x008574D0
    }
}
int main()
{
    CThingCreatureBase owner; CCreatureAction_BeetleLand a(&owner);
    if (g_owner != &owner || g_code != 0 || g_s0 != 0 || g_s1 != 0 || (g_anim == 0 || std::strcmp(g_anim, "LAND") != 0)) { std::printf("00982c70_TEST FAIL\n"); return 1; }
    std::printf("00982c70_TEST PASS\n");
    return 0;
}