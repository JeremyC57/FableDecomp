#include <cstdio>
#include <cstring>
struct CThingCreatureBase { int id; }; struct CThing { int id; };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code; static long g_s0 = 7, g_s1 = 7; static int g_setAnims;
struct CCombatActionBase { virtual void v00() { } unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = 0; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound)
    { g_owner = owner; g_target = target; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; } };
struct CCreatureAction_FinishFireballSpell : CCombatActionBase { virtual void v00() { }
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner) : CCombatActionBase(owner, 145, CSoundPair()) { SetAnims(); }
    CCreatureAction_FinishFireballSpell(CThingCreatureBase* owner, CThing* target) : CCombatActionBase(owner, target, 145, CSoundPair()) { SetAnims(); }
    void SetAnims() { ++g_setAnims; } };
int main()
{
    CThingCreatureBase owner; CThing target;
    CCreatureAction_FinishFireballSpell a(&owner, &target);
    if (g_owner != &owner || g_target != &target || g_code != 145 || g_s0 != 0 || g_s1 != 0 || g_setAnims != 1 || sizeof(a) != 0x114) { std::printf("008bce50_TEST FAIL\n"); return 1; }
    std::printf("008bce50_TEST PASS\n");
    return 0;
}