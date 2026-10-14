#include <cstdio>
// Model of CAIStateGroup_SleepOnFloor::OnActivate @ 0x00969A80: assign the creature to the floor's task, clear Sleeping.
struct CThingCreature { int id; };
struct CVillageTaskBase { CThingCreature* assigned; void AssignCreatureToTask(CThingCreature* c) { assigned = c; } };
struct CThing { unsigned long bits[9]; CVillageTaskBase* task; bool GetInterface(long type, CVillageTaskBase** out) { if (bits[type / 32] & (1 << (type % 32))) { *out = task; return true; } return false; } };
struct Group {
    CThing* floor; CThingCreature* creature; bool Sleeping;
    void OnActivate() { CVillageTaskBase* task = 0; floor->GetInterface(0xac, &task); if (task) task->AssignCreatureToTask(creature); Sleeping = false; }
};
int main()
{
    CVillageTaskBase task = { 0 }; CThing floor = {}; floor.task = &task; floor.bits[5] |= 0x1000; CThingCreature c = { 1 };
    Group g; g.floor = &floor; g.creature = &c; g.Sleeping = true;
    g.OnActivate();
    if (task.assigned != &c || g.Sleeping) { std::printf("00969a80_TEST FAIL\n"); return 1; }
    std::printf("00969a80_TEST PASS\n"); return 0;
}