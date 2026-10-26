#include <cstdio>
class CThingCreatureBase { public: bool reg; bool IsRegisteredCombatant() { return reg; } };
class CCombatManager { public: CThingCreatureBase* last; int n; void DeregisterCombatant(CThingCreatureBase* c) { last = c; ++n; } };
class CWorld { public: unsigned char _pad[0x4c]; CCombatManager* m_pCombatManager; };
class CAIBrain { public: unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; CWorld* world; CWorld* GetPWorld() { return world; } };
struct CAIStateGroup_MazeBase : CAIStateGroupBase { virtual void OnReplace(); };
void CAIStateGroup_MazeBase::OnReplace()
{ if (PBrain->Creature->IsRegisteredCombatant()) { CThingCreatureBase* creature = PBrain->Creature; CWorld* world = GetPWorld(); world->m_pCombatManager->DeregisterCombatant(creature); } }
int main()
{
    CThingCreatureBase c = { false }; CCombatManager m = { 0, 0 }; CWorld w; w.m_pCombatManager = &m; CAIBrain b; b.Creature = &c;
    CAIStateGroup_MazeBase g; g.PBrain = &b; g.world = &w;
    g.OnReplace(); if (m.n != 0) { std::printf("009540c0_TEST FAIL\n"); return 1; }
    c.reg = true; g.OnReplace(); if (m.n != 1 || m.last != &c) { std::printf("009540c0_TEST FAIL\n"); return 1; }
    std::printf("009540c0_TEST PASS\n"); return 0;
}