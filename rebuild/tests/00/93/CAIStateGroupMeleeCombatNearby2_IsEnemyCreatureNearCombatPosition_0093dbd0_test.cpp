#include <cstdio>
class CThingCreatureBase { public: bool reg; bool IsRegisteredCombatant() { return reg; } };
class CCombatManager { public: bool near; CThingCreatureBase* last; bool Combat_CheckRingRangeToTarget(CThingCreatureBase* c) { last = c; return near; } };
class CWorld { public: unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; CCombatManager* GetPCombatManager() { return m_pCombatManager; } };
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* w; CWorld* GetPWorld() { return w; } };
struct CAIStateGroup_MeleeCombatNearby2 : CAIStateGroupBase { unsigned char PEnemy[8]; bool Archer; bool IsEnemyCreatureNearCombatPosition(); };
bool CAIStateGroup_MeleeCombatNearby2::IsEnemyCreatureNearCombatPosition()
{ if (PBrain->Creature->IsRegisteredCombatant()) { CThingCreatureBase* creature = PBrain->Creature; if (GetPWorld()->GetPCombatManager()->Combat_CheckRingRangeToTarget(creature)) return true; } return false; }
int main()
{
    CThingCreatureBase c = { false }; CCombatManager m = { true, 0 }; CWorld w; w.m_pCombatManager = &m; CAIBrain b; b.Creature = &c;
    CAIStateGroup_MeleeCombatNearby2 g; g.PBrain = &b; g.w = &w;
    if (g.IsEnemyCreatureNearCombatPosition() || m.last) { std::printf("0093dbd0_TEST FAIL\n"); return 1; }
    c.reg = true; if (!g.IsEnemyCreatureNearCombatPosition() || m.last != &c) { std::printf("0093dbd0_TEST FAIL\n"); return 1; }
    m.near = false; if (g.IsEnemyCreatureNearCombatPosition()) { std::printf("0093dbd0_TEST FAIL\n"); return 1; }
    std::printf("0093dbd0_TEST PASS\n"); return 0;
}