// Behaviour test for CAIStateGroup_WorkInWarehouse::Cleanup() 0x00943B40: the task is handed back to the village only when
// the group holds a task AND the creature is a village member (interface 0x23); then PTask is cleared; the move-goods base
// Cleanup runs on every path.
#include <cstdio>

struct Thing { unsigned long UsingComponents[9]; };
struct Village { int removed; const Thing* lastThing; void* lastTask; };

static int g_baseCleanups = 0;

struct WorkInWarehouse {
    void* PTask;
    Thing* Creature;
    Village* village;
    static bool HasTC(const Thing& t, long type) { return (t.UsingComponents[type >> 5] & (1UL << (type & 31))) != 0; }
    void BaseCleanup() { ++g_baseCleanups; }
    void Cleanup()
    {
        if (PTask) {
            bool isVillageMember = HasTC(*Creature, 0x23);
            if (isVillageMember) {
                village->removed++;
                village->lastTask = PTask;
                village->lastThing = Creature;
                PTask = 0;
            }
        }
        BaseCleanup();
    }
};

int main()
{
    Thing member = { { 0 } };
    member.UsingComponents[1] = 1UL << 3;          // 0x23 = word 1, bit 3
    Thing outsider = { { 0 } };
    Village v = { 0, 0, 0 };
    int task = 7;

    WorkInWarehouse g;
    g.village = &v;

    // no task: nothing removed, base still runs
    g.PTask = 0; g.Creature = &member; g_baseCleanups = 0;
    g.Cleanup();
    if (v.removed != 0 || g_baseCleanups != 1) { std::printf("00943b40_TEST FAIL\n"); return 1; }

    // task but not a village member: task kept, base runs
    g.PTask = &task; g.Creature = &outsider; g_baseCleanups = 0;
    g.Cleanup();
    if (v.removed != 0 || g.PTask != &task || g_baseCleanups != 1) { std::printf("00943b40_TEST FAIL\n"); return 1; }

    // task and village member: removed, cleared, base runs
    g.Creature = &member; g_baseCleanups = 0;
    g.Cleanup();
    if (v.removed != 1 || v.lastTask != &task || v.lastThing != &member || g.PTask != 0 || g_baseCleanups != 1) {
        std::printf("00943b40_TEST FAIL\n"); return 1;
    }
    std::printf("00943b40_TEST PASS\n");
    return 0;
}