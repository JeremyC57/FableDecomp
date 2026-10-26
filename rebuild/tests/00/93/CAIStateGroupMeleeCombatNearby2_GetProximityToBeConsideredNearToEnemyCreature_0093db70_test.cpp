#include <cstdio>
#include <cmath>
struct CCreatureDefModel { unsigned char _pad[0x34]; float m_radius; };
class CThingCreatureBase { public: unsigned char _pad[0x60]; CCreatureDefModel* m_pDef; };
class CCombatManager { public: bool valid; bool CCombatWheel_ValidateSequencePosition(CThingCreatureBase*) { return valid; } float Combat_ProximityCalculation(CThingCreatureBase*) { return 7.5f; } };
class CWorld { public: unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; CCombatManager* GetPCombatManager() { return m_pCombatManager; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* w; CWorld* GetPWorld() { return w; } };
struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase { unsigned char PEnemy[8]; bool Archer; float GetProximityToBeConsideredNearToEnemyCreature(); };
float CAIStateGroup_MeleeCombatNearby2::GetProximityToBeConsideredNearToEnemyCreature()
{
    CThingCreatureBase* creature = PBrain->Creature;
    if (GetPWorld()->GetPCombatManager()->CCombatWheel_ValidateSequencePosition(creature)) { CThingCreatureBase* combatant = PBrain->Creature; return GetPWorld()->GetPCombatManager()->Combat_ProximityCalculation(combatant); }
    return PBrain->Creature->m_pDef->m_radius * 2.0f * 0.8333333f + 1.0f;
}
int main()
{
    CCreatureDefModel d; d.m_radius = 3.0f; CThingCreatureBase c; c.m_pDef = &d; CCombatManager m = { true }; CWorld w; w.m_pCombatManager = &m; CAIBrain b; b.Creature = &c;
    CAIStateGroup_MeleeCombatNearby2 g; g.PBrain = &b; g.w = &w;
    if (g.GetProximityToBeConsideredNearToEnemyCreature() != 7.5f) { std::printf("0093db70_TEST FAIL\n"); return 1; }
    m.valid = false; if (std::fabs(g.GetProximityToBeConsideredNearToEnemyCreature() - 6.0f) > 1e-5f) { std::printf("0093db70_TEST FAIL\n"); return 1; }
    std::printf("0093db70_TEST PASS\n"); return 0;
}