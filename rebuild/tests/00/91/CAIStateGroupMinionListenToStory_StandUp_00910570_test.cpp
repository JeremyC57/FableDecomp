// Behaviour test for 0x00910570 CAIStateGroup_MinionListenToStory::StandUp: only a minion in creature mode 10 (sitting on
// the floor) is stood up; mode 10 is removed whether or not the get-up action could start, and the brain output (type
// bit 8 + a clone of the ST_STAND_UP_FROM_FLOOR action) is set only when the creature is free to perform it.
#include <cstdio>
#include <cstring>

struct Action { const char* anim; long priority; long sitModeType; bool flag; };
struct BrainOutput { unsigned int types; bool hasAction; Action action; BrainOutput() : types(0), hasAction(false) {} };
struct Creature { unsigned int currentMode; bool free; int removedMode; Creature() : currentMode(0), free(false), removedMode(-1) {} };
struct Brain { Creature* creature; bool outputSet; BrainOutput output; };

static void StandUp(Brain& brain)
{
    Creature* modes = brain.creature;
    if (modes->currentMode == 10) {
        BrainOutput output;
        Action action = { "ST_STAND_UP_FROM_FLOOR", 0x32, 0, false };
        if (brain.creature->free) {
            output.types |= 8;
            output.hasAction = true;
            output.action = action;
            brain.outputSet = true;
            brain.output = output;
        }
        modes->removedMode = 10;
        modes->currentMode = 0;
    }
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); g_fail = 1; } }

int main()
{
    Creature sitting; sitting.currentMode = 10; sitting.free = true;
    Brain b1 = { &sitting, false, BrainOutput() };
    StandUp(b1);
    Check(b1.outputSet && b1.output.types == 8 && b1.output.hasAction, "free sitter gets the perform-action output");
    Check(std::strcmp(b1.output.action.anim, "ST_STAND_UP_FROM_FLOOR") == 0 && b1.output.action.priority == 0x32,
          "stand-up animation, priority 0x32");
    Check(sitting.removedMode == 10, "mode 10 removed");

    Creature busy; busy.currentMode = 10; busy.free = false;
    Brain b2 = { &busy, false, BrainOutput() };
    StandUp(b2);
    Check(!b2.outputSet && busy.removedMode == 10, "busy sitter: no output, mode still removed");

    Creature standing; standing.currentMode = 3; standing.free = true;
    Brain b3 = { &standing, false, BrainOutput() };
    StandUp(b3);
    Check(!b3.outputSet && standing.removedMode == -1, "other modes untouched");

    if (!g_fail) std::printf("00910570_TEST PASS\n");
    else std::printf("00910570_TEST FAIL\n");
    return g_fail;
}