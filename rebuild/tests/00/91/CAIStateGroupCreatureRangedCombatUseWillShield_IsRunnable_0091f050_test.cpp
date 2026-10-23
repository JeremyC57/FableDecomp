// Behaviour test for 0091f050: body copied from src/0091f050.cpp (CAIStateGroup_CreatureRangedCombatUseWillShield::IsRunnable), the callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThingCreatureBase;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* FindInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
static bool g_melee = false, g_toAttack = false; static int g_meleeCalls = 0, g_attackCalls = 0; static CThing* g_attackCreature = 0; static CThingCreatureBase* g_attackTarget = 0;
bool __fastcall IsThingCarryingMeleeWeapon(CThing* thing) { ++g_meleeCalls; return g_melee; }
bool __fastcall IsToAttackTarget(CThing* creature, CThingCreatureBase* target) { ++g_attackCalls; g_attackCreature = creature; g_attackTarget = target; return g_toAttack; }
static CThingCreatureBase* g_target = 0;
struct CTCTargeting {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {}
    virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {}
    virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {} virtual void v24() {}
    virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {}
    virtual CThingCreatureBase* GetTarget() { return g_target; }
};
struct CAIBrain { unsigned char _pad[0x18]; CThing* Creature; };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase() {}
    CAIBrain* PBrain; long ScriptAIPriority; void* Name_PStringData; unsigned char Terminated; unsigned char Priority; unsigned char Enabled; unsigned char _pad_0x13;
};
struct CAIStateGroup_CreatureRangedCombatUseWillShield : CAIStateGroupBase { virtual bool IsRunnable(); };
bool CAIStateGroup_CreatureRangedCombatUseWillShield::IsRunnable()
{
    if (IsThingCarryingMeleeWeapon(PBrain->Creature)) {
        CThing* creature = PBrain->Creature;
        if (creature->HasInterface(8)) {
            CThingCreatureBase* target = ((CTCTargeting*)creature->FindInterface(8))->GetTarget();
            if (target != 0 && IsToAttackTarget(PBrain->Creature, target) && !PBrain->Creature->HasInterface(0x93))
                return true;
        }
    }
    return false;
}
int main()
{
    CThing creature; memset(&creature, 0, sizeof(creature)); CTCTargeting targeting; CThingCreatureBase* enemy = (CThingCreatureBase*)0x5000;
    CInterfaceMap::SEntry entries[2] = { { 8, (CTCBase*)&targeting }, { 0x30, (CTCBase*)0x99 } };
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    CAIBrain brain; brain.Creature = &creature; CAIStateGroup_CreatureRangedCombatUseWillShield g; g.PBrain = &brain;
    // 1. no melee weapon: false
    CHECK(g.IsRunnable() == false && g_meleeCalls == 1 && g_attackCalls == 0);
    // 2. melee weapon but no targeting component
    g_melee = true; CHECK(g.IsRunnable() == false && g_attackCalls == 0);
    // 3. targeting present, no target
    creature.m_presentBits[0] = 1UL << 8; CHECK(g.IsRunnable() == false && g_attackCalls == 0);
    // 4. target present but not to be attacked
    g_target = enemy; CHECK(g.IsRunnable() == false && g_attackCalls == 1 && g_attackCreature == &creature && g_attackTarget == enemy);
    // 5. to attack, no shield (0x93): true
    g_toAttack = true; CHECK(g.IsRunnable() == true && g_attackCalls == 2);
    // 6. already shielded (0x93 present): false
    creature.m_presentBits[0x93 >> 5] |= 1UL << (0x93 & 31); CHECK(g.IsRunnable() == false && g_attackCalls == 3);
    if (fails == 0) printf("0091f050_TEST PASS\n"); else printf("0091f050_TEST FAIL\n");
    return fails;
}