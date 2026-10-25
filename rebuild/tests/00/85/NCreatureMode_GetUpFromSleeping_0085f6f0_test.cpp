// Behaviour test for NCreatureMode::GetUpFromSleeping (0x0085F6F0): in the sleep-in-bed mode (type 0x0D) the mode is
// left with a get-out-of-bed action and the result is true; otherwise a free creature gets the action through its brain
// (flag 8 plus a clone) or directly without a brain, a busy creature gets nothing, and the result is false. Every
// action built is destroyed again.
#include <cstdio>

static int g_live = 0;      // constructed minus destroyed actions
static int g_clones = 0;

struct Action {
    const void* bed;
    Action(const void* b) : bed(b) { ++g_live; }
    Action(const Action& o) : bed(o.bed) { ++g_live; }
    ~Action() { --g_live; }
    Action* Clone() const { ++g_clones; return new Action(*this); }
};

struct Mode { long type; int removedWith; };

struct Creature {
    Mode mode;
    bool free;
    int setDirect;
    bool IsFreeToPerformAction(const Action&) const { return free; }
    void SetCurrentAction(const Action&) { ++setDirect; }
};

struct BrainOutput {
    unsigned long Flags;
    Action* PAction;
    BrainOutput() : Flags(0), PAction(0) { }
    ~BrainOutput() { delete PAction; }
};

struct Brain {
    int posted;
    unsigned long lastFlags;
    void SetBrainOutput(const BrainOutput& o) { ++posted; lastFlags = o.Flags; }
};

static bool GetUpFromSleeping(Creature& creature, int& bed, Brain* brain)
{
    Mode& mode = creature.mode;
    if (mode.type == 0x0d) {
        Action temp(&bed);
        ++mode.removedWith;
        return true;
    } else {
        Action action(&bed);
        if (creature.IsFreeToPerformAction(action)) {
            if (brain) {
                BrainOutput output;
                output.Flags |= 8;
                output.PAction = action.Clone();
                brain->SetBrainOutput(output);
            } else
                creature.SetCurrentAction(action);
        }
        return false;
    }
}

int main()
{
    int fails = 0;
    int bed = 0;
    Brain brain = { 0, 0 };
    Creature sleeping = { { 0x0d, 0 }, true, 0 };
    if (!GetUpFromSleeping(sleeping, bed, &brain) || sleeping.mode.removedWith != 1 || brain.posted != 0) ++fails;
    Creature awake = { { 0x02, 0 }, true, 0 };
    if (GetUpFromSleeping(awake, bed, &brain) || brain.posted != 1 || brain.lastFlags != 8 || g_clones != 1) ++fails;
    if (GetUpFromSleeping(awake, bed, 0) || awake.setDirect != 1 || brain.posted != 1) ++fails;
    Creature busy = { { 0x02, 0 }, false, 0 };
    if (GetUpFromSleeping(busy, bed, &brain) || busy.setDirect != 0 || brain.posted != 1 || g_clones != 1) ++fails;
    if (g_live != 0) ++fails;
    if (fails) { std::printf("0085f6f0_TEST FAIL\n"); return 1; }
    std::printf("0085f6f0_TEST PASS\n");
    return 0;
}