// Test for 0x00733580 CTCCombat::RemoveFromCombat: links the source object; ClearCombatVariables always runs once,
// and the parent creature is handed to DeregisterCombatant (by reference) only when it is in melee as an attacker.
#include <stdio.h>
static int g_clear, g_dereg;
static const void* g_who;
class CThingCreatureBase {
public:
    bool attacker;
    bool IsInMeleeAsAttacker() const;
};
bool CThingCreatureBase::IsInMeleeAsAttacker() const { return attacker; }
class CCombatManager {
public:
    void DeregisterCombatant(CThingCreatureBase& creature);
};
void CCombatManager::DeregisterCombatant(CThingCreatureBase& creature) { ++g_dereg; g_who = &creature; }
class CTCCombat {
public:
    virtual void v00();
    void*               ParentThing;
    bool                ToBeRemoved;
    bool                IsInGlobalUpdateTCs;
    long                pad[5];
    CCombatManager*     CombatManager;
    CThingCreatureBase* ParentCreature;
    void RemoveFromCombat();
private:
    void ClearCombatVariables();
};
void CTCCombat::v00() { }
void CTCCombat::ClearCombatVariables() { ++g_clear; }
int main()
{
    CThingCreatureBase cr; CCombatManager mg; CTCCombat c;
    c.CombatManager = &mg; c.ParentCreature = &cr;
    cr.attacker = false; c.RemoveFromCombat();
    bool ok = g_clear == 1 && g_dereg == 0;
    cr.attacker = true; c.RemoveFromCombat();
    ok = ok && g_clear == 2 && g_dereg == 1 && g_who == &cr;
    if (!ok) { puts("00733580_TEST FAIL"); return 1; }
    puts("00733580_TEST PASS");
    return 0;
}
