// Behaviour test for 0x0085FF10 NCreatureMode::GetUpFromSitting(CThingCreatureBase&, long, const CCharString&, CAIBrain*)
// The model mirrors the source: when the creature's current mode has the sitting mode type, that mode is ended with a
// get-up action carrying (anim, type, true) and the call returns true; otherwise a plain get-up action (anim, 0, false) is
// offered: when the creature is free to perform it, a brain gets an output of type 8 holding a clone, or without a brain
// the action is set directly; that path always returns false. Every action object is destroyed.
#include <cstdio>
#include <cstring>

static int g_actions_alive = 0;
static int g_clones_alive = 0;

struct Action {
    const char* anim;
    long type;
    bool flag;
    bool clone;
    Action(const char* a, long t, bool f) : anim(a), type(t), flag(f), clone(false) { ++g_actions_alive; }
    Action(const Action& o) : anim(o.anim), type(o.type), flag(o.flag), clone(true) { ++g_clones_alive; }
    ~Action() { if (clone) --g_clones_alive; else --g_actions_alive; }
};

struct Creature {
    long current_mode_type;
    bool free_to_act;
    int mode_removals;
    long removal_type;
    bool removal_flag;
    int actions_set;
    const char* current_anim;
};
struct BrainOutput { unsigned long types; Action* action; };
struct Brain { int outputs; unsigned long last_types; const char* last_anim; };

static bool GetUpFromSitting(Creature& creature, long sit_mode_type, const char* anim, Brain* brain)
{
    if (creature.current_mode_type == sit_mode_type) {
        Action a(anim, sit_mode_type, true);
        ++creature.mode_removals;
        creature.removal_type = a.type;
        creature.removal_flag = a.flag;
        return true;
    }
    {
        Action action(anim, 0, false);
        if (creature.free_to_act) {
            if (brain != 0) {
                BrainOutput output = { 0, 0 };
                output.types |= 8;
                output.action = new Action(action);
                ++brain->outputs;
                brain->last_types = output.types;
                brain->last_anim = output.action->anim;
                delete output.action;           // the counted pointer drops its reference with the output
            } else {
                ++creature.actions_set;
                creature.current_anim = action.anim;
            }
        }
    }
    return false;
}

int main()
{
    int fail = 0;
    Creature c = { 7, true, 0, 0, false, 0, 0 };
    Brain b = { 0, 0, 0 };
    if (!GetUpFromSitting(c, 7, "STAND_UP", &b)) fail |= 1;
    if (c.mode_removals != 1 || c.removal_type != 7 || !c.removal_flag || b.outputs != 0) fail |= 2;
    if (GetUpFromSitting(c, 9, "STAND_UP", &b)) fail |= 4;                  // not sitting in that mode
    if (b.outputs != 1 || b.last_types != 8 || std::strcmp(b.last_anim, "STAND_UP") != 0) fail |= 8;
    if (GetUpFromSitting(c, 9, "GET_UP", 0)) fail |= 16;                      // no brain: set directly
    if (c.actions_set != 1 || std::strcmp(c.current_anim, "GET_UP") != 0) fail |= 32;
    c.free_to_act = false;
    if (GetUpFromSitting(c, 9, "GET_UP", &b)) fail |= 64;
    if (b.outputs != 1 || c.actions_set != 1) fail |= 128;                    // busy: nothing offered
    if (g_actions_alive != 0 || g_clones_alive != 0) fail |= 256;
    if (fail) { std::printf("0085ff10_TEST FAIL %d\n", fail); return 1; }
    std::printf("0085ff10_TEST PASS\n");
    return 0;
}