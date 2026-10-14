#include <cstdio>

// Model of CCombatActionBase::GetDamageMultiplier @ 0x00856EA0

static int g_deletes = 0;
struct CMeleeCombatAbilityDef { virtual void v00() {} virtual void Delete() { ++g_deletes; } long m_references; unsigned char _pad08[0x44 - 8]; float DamageMultiplier; };
struct CCountedAbilityPointer {
    CMeleeCombatAbilityDef* m_pItem;
    CCountedAbilityPointer(CMeleeCombatAbilityDef* p) : m_pItem(p) { if (m_pItem) ++m_pItem->m_references; }
    CCountedAbilityPointer(const CCountedAbilityPointer& o) : m_pItem(o.m_pItem) { if (m_pItem != 0) ++m_pItem->m_references; }
    ~CCountedAbilityPointer() { if (m_pItem != 0) { if (--m_pItem->m_references == 0) m_pItem->Delete(); } }
    CMeleeCombatAbilityDef* GetPItem() const { return m_pItem; }
    CMeleeCombatAbilityDef* operator->() const { return m_pItem; }
};

struct CCombatActionBase {
    CCountedAbilityPointer PMeleeAbilityDef;
    CCombatActionBase(CMeleeCombatAbilityDef* p) : PMeleeAbilityDef(p) {}
    CCountedAbilityPointer GetPMeleeAbilityDef() const { return PMeleeAbilityDef; }
    float GetDamageMultiplier() const { if (GetPMeleeAbilityDef().GetPItem() != 0) return GetPMeleeAbilityDef()->DamageMultiplier; return 1.0f; }
};
int main()
{
    CMeleeCombatAbilityDef def; def.m_references = 0; def.DamageMultiplier = 2.5f;
    { CCombatActionBase a(&def); if (a.GetDamageMultiplier() != 2.5f || def.m_references != 1 || g_deletes != 0) { std::printf("00856ea0_TEST FAIL\n"); return 1; } }
    if (def.m_references != 0 || g_deletes != 1) { std::printf("00856ea0_TEST FAIL\n"); return 1; }
    CCombatActionBase none(0);
    if (none.GetDamageMultiplier() != 1.0f) { std::printf("00856ea0_TEST FAIL\n"); return 1; }
    std::printf("00856ea0_TEST PASS\n"); return 0;
}