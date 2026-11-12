// Behaviour test for CAIStateGroup_MinionReactToCrime::OnReplace (0x00958B20): a model of the replacement cleanup.
#include <cstdio>
struct Thing { bool dead; int forceOpen; Thing() : dead(false), forceOpen(0) {} };
struct Message { Thing* originator; };
struct Opinion { long defDeedMask; long mask; int calls; void SetOpinionDeedMask(long m) { mask = m; ++calls; } };
struct Creature : Thing { Opinion* opinion; };
struct Group {
    Creature* creature; long OpinionDeedHandle; bool GuardTold; bool DeedMaskApplied; Thing* ForceOpenBuilding;
    Message* message; int removed;
    Message* GetSomeoneElseGettingHelpMessage(long) { return message; }
    void OnReplace()
    {
        if (ForceOpenBuilding && !ForceOpenBuilding->dead)
            --ForceOpenBuilding->forceOpen;
        if (!GuardTold) {
            Message* m = GetSomeoneElseGettingHelpMessage(OpinionDeedHandle);
            if (m && m->originator == creature)
                ++removed;
        }
        GuardTold = false;
        if (DeedMaskApplied) {
            if (creature->opinion) {
                creature->opinion->SetOpinionDeedMask(creature->opinion->defDeedMask);
                DeedMaskApplied = false;
            }
        }
    }
};
int main()
{
    Opinion op; op.defDeedMask = 12; op.mask = 0; op.calls = 0;
    Creature c; c.opinion = &op;
    Thing building; building.forceOpen = 2;
    Message own; own.originator = &c;
    Group g; g.creature = &c; g.OpinionDeedHandle = 5; g.GuardTold = false; g.DeedMaskApplied = true; g.ForceOpenBuilding = &building;
    g.message = &own; g.removed = 0;
    g.OnReplace();
    if (building.forceOpen != 1 || g.removed != 1 || op.mask != 12 || g.DeedMaskApplied || g.GuardTold) { std::printf("00958b20_TEST FAIL\n"); return 1; }
    Thing other; Message foreign; foreign.originator = &other;
    building.dead = true; g.GuardTold = true; g.message = &foreign; g.DeedMaskApplied = true; c.opinion = 0;
    g.OnReplace();   // dead building, guard told, no opinion component: nothing but GuardTold cleared
    if (building.forceOpen != 1 || g.removed != 1 || !g.DeedMaskApplied || g.GuardTold) { std::printf("00958b20_TEST FAIL\n"); return 1; }
    g.GuardTold = false; g.OnReplace();   // someone else's message: kept
    if (g.removed != 1) { std::printf("00958b20_TEST FAIL\n"); return 1; }
    std::printf("00958b20_TEST PASS\n");
    return 0;
}