#include <cstdio>
#include <cstring>
struct CMeleeCombatAbilityDef { unsigned char _pad[0x64]; unsigned char EvadeAllHits; unsigned char CauseRecoil; };
static bool g_flourish;
struct CCombatActionBase { virtual void v00() { } virtual bool IsMeleeFlourish() const { return g_flourish; } unsigned char _pad04[0xb8 - 4]; CMeleeCombatAbilityDef* PMeleeAbilityDef; unsigned char _padbc[0x114 - 0xbc]; };

struct CCreatureAction_InterruptableMidAttackAutoTurn : CCombatActionBase { virtual bool IsToCauseRecoil() const; };
bool CCreatureAction_InterruptableMidAttackAutoTurn::IsToCauseRecoil() const { if (IsMeleeFlourish() && PMeleeAbilityDef->CauseRecoil) return true; return false; }
int main()
{
    CMeleeCombatAbilityDef def; std::memset(&def, 0, sizeof(def));
    CCreatureAction_InterruptableMidAttackAutoTurn a; a.PMeleeAbilityDef = &def;
    g_flourish = false; def.CauseRecoil = 1; if (a.IsToCauseRecoil()) { std::printf("0087c850_TEST FAIL\n"); return 1; }
    g_flourish = true; a.PMeleeAbilityDef = &def; if (!a.IsToCauseRecoil()) { std::printf("0087c850_TEST FAIL\n"); return 1; }
    def.CauseRecoil = 0; if (a.IsToCauseRecoil()) { std::printf("0087c850_TEST FAIL\n"); return 1; }
    if ((char*)&a.PMeleeAbilityDef - (char*)&a != 0xb8) { std::printf("0087c850_TEST FAIL\n"); return 1; }
    std::printf("0087c850_TEST PASS\n");
    return 0;
}