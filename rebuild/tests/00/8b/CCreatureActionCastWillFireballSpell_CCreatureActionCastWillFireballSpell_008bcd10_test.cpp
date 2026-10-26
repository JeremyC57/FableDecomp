#include <cstdio>
#include <cstring>
class CCharString { public: char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const char* g_anim;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_pData; } };

struct CCreatureAction_CastSpellBase : CCombatActionBase { unsigned char ImmediateCast; unsigned char OneAnimCast;
    CCreatureAction_CastSpellBase(CThingCreatureBase* owner, long actionCode) : CCombatActionBase(owner, actionCode, CSoundPair()), ImmediateCast(0), OneAnimCast(0) { } };
class CCreatureAction_CastWillFireballSpell : public CCreatureAction_CastSpellBase { public: long LevelToCast; virtual void v00() {} CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner); };
CCreatureAction_CastWillFireballSpell::CCreatureAction_CastWillFireballSpell(CThingCreatureBase* owner) : CCreatureAction_CastSpellBase(owner, 150), LevelToCast(-1) { }
int main()
{
    CThingCreatureBase owner; CCreatureAction_CastWillFireballSpell a(&owner);
    if (g_owner != &owner || g_code != 150 || a.ImmediateCast != 0 || a.OneAnimCast != 0 || a.LevelToCast != -1 || (char*)&a.LevelToCast - (char*)&a != 0x118) { std::printf("008bcd10_TEST FAIL\n"); return 1; }
    std::printf("008bcd10_TEST PASS\n");
    return 0;
}