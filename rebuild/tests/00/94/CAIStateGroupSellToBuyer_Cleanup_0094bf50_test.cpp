// Behaviour test for CAIStateGroup_SellToBuyer::Cleanup 0x0094BF50: cleanup stops the brain's own creature speaking (the creature at
// brain +0x18) exactly once, and no other creature.
#include <cstdio>
struct CThingCreatureBase { int speaking; };
namespace NCreatureAI { static void StopSpeaking(CThingCreatureBase& c) { c.speaking = 0; } }
struct CAIBrain { unsigned char Before[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { void* VPtr; CAIBrain* PBrain; CThingCreatureBase& GetCreature() const { return *PBrain->Creature; } };
struct CAIStateGroup_SellToBuyer : CAIStateGroupBase { void Cleanup() { NCreatureAI::StopSpeaking(GetCreature()); } };
int main()
{
    CThingCreatureBase me = { 1 }, other = { 1 };
    CAIBrain brain; brain.Creature = &me;
    CAIStateGroup_SellToBuyer g; g.PBrain = &brain;
    g.Cleanup();
    if (me.speaking != 0 || other.speaking != 1) { std::printf("0094bf50_TEST FAIL\n"); return 1; }
    std::printf("0094bf50_TEST PASS\n");
    return 0;
}