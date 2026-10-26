#include <cstdio>
class CThingCreatureBase { public: bool perf, free; int finished; bool IsPerformingAnyAction() { return perf; } bool IsFreeToFinishAction() { return free; } void FinishCurrentAction() { ++finished; } };
struct CAIStateGroup_MeleeCombatNearbyBase { static void UpdateCurrentAction(CThingCreatureBase* creature); };
void CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(CThingCreatureBase* creature) { if (creature->IsPerformingAnyAction() && creature->IsFreeToFinishAction()) creature->FinishCurrentAction(); }
int main()
{
    CThingCreatureBase c = { true, true, 0 };
    CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(&c); c.free = false; CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(&c); c.perf = false; c.free = true; CAIStateGroup_MeleeCombatNearbyBase::UpdateCurrentAction(&c);
    if (c.finished != 1) { std::printf("006e8400_TEST FAIL\n"); return 1; }
    std::printf("006e8400_TEST PASS\n"); return 0;
}