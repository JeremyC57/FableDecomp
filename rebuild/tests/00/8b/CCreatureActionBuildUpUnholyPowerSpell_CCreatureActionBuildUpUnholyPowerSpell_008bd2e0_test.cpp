#include <cstdio>
#include <cstring>
class CCharString { public: char* m_pData; CCharString(const char* t) : m_pData((char*)t) { } ~CCharString() { m_pData = 0; } };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const char* g_anim;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_pData; } };

class CCreatureAction_BuildUpUnholyPowerSpell : public CCombatActionBase { public: virtual void v00() {} CCreatureAction_BuildUpUnholyPowerSpell(CThingCreatureBase* owner, CCharString animName); };
CCreatureAction_BuildUpUnholyPowerSpell::CCreatureAction_BuildUpUnholyPowerSpell(CThingCreatureBase* owner, CCharString animName) : CCombatActionBase(owner, 150, CSoundPair()) { SetAnimationName(animName); }
int main()
{
    CThingCreatureBase owner; CCharString n("SPELL_ANIM"); CCreatureAction_BuildUpUnholyPowerSpell a(&owner, n);
    if (g_owner != &owner || g_code != 150 || g_s0 != 0 || g_s1 != 0 || g_anim == 0 || std::strcmp(g_anim, "SPELL_ANIM") != 0) { std::printf("008bd2e0_TEST FAIL\n"); return 1; }
    std::printf("008bd2e0_TEST PASS\n");
    return 0;
}