#include <cstdio>
struct CThingCreatureBase { bool perf, free; int finished; bool IsPerformingAnyAction() { return perf; } bool IsFreeToFinishAction() { return free; } void FinishCurrentAction() { ++finished; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase { unsigned char PEnemy[8]; void UpdateCurrentAction(); };
void CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction() { if (PBrain->Creature->IsPerformingAnyAction() && PBrain->Creature->IsFreeToFinishAction()) PBrain->Creature->FinishCurrentAction(); }
int main()
{
    CThingCreatureBase c = { true, true, 0 }; CAIBrain b; b.Creature = &c; CAIStateGroup_MeleeCombatNearbyBase g; g.PBrain = &b;
    g.UpdateCurrentAction(); c.free = false; g.UpdateCurrentAction(); c.perf = false; c.free = true; g.UpdateCurrentAction();
    if (c.finished != 1) { std::printf("0093d140_TEST FAIL\n"); return 1; }
    std::printf("0093d140_TEST PASS\n"); return 0;
}