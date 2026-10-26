#include <cstdio>
#include <cstring>
class CMeleeCombatAbilityDef { public: unsigned char _pad[0x64]; unsigned char EvadeAllHits; unsigned char CauseRecoil; };
static bool g_flourish;
class CCombatActionBase { public: virtual void v00() { } virtual bool IsMeleeFlourish() const { return g_flourish; } unsigned char _pad04[0xb8 - 4]; CMeleeCombatAbilityDef* PMeleeAbilityDef; unsigned char _padbc[0x114 - 0xbc]; };

class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public: virtual bool IsToEvadeAllHits() const; };
bool CCreatureAction_InterruptableMidAttackAutoTurn::IsToEvadeAllHits() const { if (IsMeleeFlourish() && PMeleeAbilityDef->EvadeAllHits) return true; return false; }
int main()
{
    CMeleeCombatAbilityDef def; std::memset(&def, 0, sizeof(def));
    CCreatureAction_InterruptableMidAttackAutoTurn a; a.PMeleeAbilityDef = &def;
    g_flourish = false; def.EvadeAllHits = 1; if (a.IsToEvadeAllHits()) { std::printf("0087c880_TEST FAIL\n"); return 1; }
    g_flourish = true; a.PMeleeAbilityDef = &def; if (!a.IsToEvadeAllHits()) { std::printf("0087c880_TEST FAIL\n"); return 1; }
    def.EvadeAllHits = 0; if (a.IsToEvadeAllHits()) { std::printf("0087c880_TEST FAIL\n"); return 1; }
    if ((char*)&a.PMeleeAbilityDef - (char*)&a != 0xb8) { std::printf("0087c880_TEST FAIL\n"); return 1; }
    std::printf("0087c880_TEST PASS\n");
    return 0;
}