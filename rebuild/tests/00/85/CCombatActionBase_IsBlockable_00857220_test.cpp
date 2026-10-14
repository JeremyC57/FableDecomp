#include <cstdio>

// Model of CCombatActionBase::IsBlockable @ 0x00857220
struct CMeleeCombatAbilityDef { int id; };
struct CThingCreatureBase { unsigned char _pad[0x30]; unsigned long Flags30; };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
struct CCombatActionBase {
    virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature; unsigned char _pad10[0xb8 - 0x10]; CMeleeCombatAbilityDef* PMeleeAbilityDef; unsigned char _padbc[0x114 - 0xbc];
    bool IsBlockable() const { if (PCreature.GetPItem()->Flags30 & 0x10) return false; bool blockable = (PMeleeAbilityDef == 0); return blockable; }
};
int main()
{
    CThingCreatureBase c; CMeleeCombatAbilityDef def; CCombatActionBase a; a.PCreature.m_pItem = &c;
    c.Flags30 = 0x10; a.PMeleeAbilityDef = 0;
    if (a.IsBlockable()) { std::printf("00857220_TEST FAIL\n"); return 1; }
    c.Flags30 = 0;
    if (!a.IsBlockable()) { std::printf("00857220_TEST FAIL\n"); return 1; }
    a.PMeleeAbilityDef = &def;
    if (a.IsBlockable()) { std::printf("00857220_TEST FAIL\n"); return 1; }
    std::printf("00857220_TEST PASS\n"); return 0;
}