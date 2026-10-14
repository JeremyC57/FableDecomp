#include <cstdio>
#include <cstring>
struct CCharString { char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
struct CThingCreatureBase { int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const char* g_anim;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_pData; } };

struct CCreatureAction_TrollVulnerableInto : CCombatActionBase { virtual void v00() {} CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner); };
CCreatureAction_TrollVulnerableInto::CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner) : CCombatActionBase(owner, 250, CSoundPair()) {
    {
        CCharString name("VULNERABLE_INTO");         // conversion-constructor length default (-1), re-addressed with lea
        SetAnimationName(name);            // 0x008574D0
    }
}
int main()
{
    CThingCreatureBase owner; CCreatureAction_TrollVulnerableInto a(&owner);
    if (g_owner != &owner || g_code != 250 || g_s0 != 0 || g_s1 != 0 || (g_anim == 0 || std::strcmp(g_anim, "VULNERABLE_INTO") != 0)) { std::printf("008e4420_TEST FAIL\n"); return 1; }
    std::printf("008e4420_TEST PASS\n");
    return 0;
}