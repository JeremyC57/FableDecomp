#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; }; class CThing { public: int id; };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7; static int g_setAnims;
class CCombatActionBase { public: virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = 0; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; } };
class CCreatureAction_FinishFireballSpell : public CCombatActionBase { public: virtual void v00() { }
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner) : CCombatActionBase(owner, 145, CSoundPair()) { SetAnims(); }
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, target, 145, CSoundPair()) { SetAnims(); }
    void SetAnims() { ++g_setAnims; } };
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_FinishFireballSpell a(&owner);
    if (g_owner != &owner || g_target != 0 || g_code != 145 || g_s0 != 0 || g_s1 != 0 || g_setAnims != 1 || sizeof(a) != 0x114) { std::printf("008bce10_TEST FAIL\n"); return 1; }
    std::printf("008bce10_TEST PASS\n");
    return 0;
}