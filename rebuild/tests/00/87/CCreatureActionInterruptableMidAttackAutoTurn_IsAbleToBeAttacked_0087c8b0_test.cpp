#include <cstdio>
static int g_deletes; static bool g_evade, g_recoil;
class CMeleeCombatAbilityDef { public: virtual void v00() { } virtual void Delete() { ++g_deletes; } long m_references; };
struct CCountedAbilityPointer { CMeleeCombatAbilityDef* m_pItem; CCountedAbilityPointer(CMeleeCombatAbilityDef* p) : m_pItem(p) { }
    CCountedAbilityPointer(const CCountedAbilityPointer& other) : m_pItem(other.m_pItem) { if (m_pItem != 0) ++m_pItem->m_references; }
    ~CCountedAbilityPointer() { if (m_pItem != 0) { if (--m_pItem->m_references == 0) m_pItem->Delete(); } }
    CMeleeCombatAbilityDef* GetPItem() const { return m_pItem; } };
class CCombatActionBase { public: virtual void v00() { } virtual bool IsToEvadeAllHits() const { return g_evade; } virtual bool IsToCauseRecoil() const { return g_recoil; }
    unsigned char _pad04[0xb8 - 4]; CCountedAbilityPointer PMeleeAbilityDef; unsigned char _padbc[0x114 - 0xbc];
    CCombatActionBase(CMeleeCombatAbilityDef* p) : PMeleeAbilityDef(p) { }
    CCountedAbilityPointer GetPMeleeAbilityDef() const { return PMeleeAbilityDef; } };
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public: CCreatureAction_InterruptableMidAttackAutoTurn(CMeleeCombatAbilityDef* p) : CCombatActionBase(p) { } bool IsAbleToBeAttacked() const; };
bool CCreatureAction_InterruptableMidAttackAutoTurn::IsAbleToBeAttacked() const
{
    if (IsToEvadeAllHits() || IsToCauseRecoil()) return false;
    bool hasAbility = GetPMeleeAbilityDef().GetPItem() != 0;
    return hasAbility ? false : true;
}
int main()
{
    CMeleeCombatAbilityDef def; def.m_references = 1;
    CCreatureAction_InterruptableMidAttackAutoTurn a(&def); CCreatureAction_InterruptableMidAttackAutoTurn b(0);
    g_evade = true; g_recoil = false; if (a.IsAbleToBeAttacked() || b.IsAbleToBeAttacked()) { std::printf("0087c8b0_TEST FAIL\n"); return 1; }
    g_evade = false; g_recoil = true; if (a.IsAbleToBeAttacked() || b.IsAbleToBeAttacked()) { std::printf("0087c8b0_TEST FAIL\n"); return 1; }
    g_recoil = false; if (a.IsAbleToBeAttacked() || !b.IsAbleToBeAttacked() || def.m_references != 1 || g_deletes != 0) { std::printf("0087c8b0_TEST FAIL\n"); return 1; }
    { CCountedAbilityPointer last(&def); a.PMeleeAbilityDef.m_pItem = 0; }
    if (g_deletes != 1) { std::printf("0087c8b0_TEST FAIL\n"); return 1; }
    std::printf("0087c8b0_TEST PASS\n");
    return 0;
}