// Behaviour test for CWillButtonHelperTimer::FrameUpdate (0x008EDDD0): the AI and player charge paths, level-ups against
// the timing table, and the state transitions.
#include <cstdio>

struct Inventory { bool quickPending; bool buttonSet; bool quickInQueue; int cleared; };
static bool g_buttonInQueue = true;

struct Timer {
    long ButtonHeldDownTimer, WillAccumulatedLevel; bool creatureAlive; bool dead; long MyAbility, CurrentState;
    const float* Timings; float CurMaxLevels; bool WillIncrementedFlag, UpdateAccLevel, IsAICasting, IsAIHoldingDown;
    long LevelToCast; Inventory* inv;
    void FrameUpdate()
    {
        WillIncrementedFlag = false;
        if (!creatureAlive) return;
        if (dead) return;
        if (IsAICasting) {
            if (CurrentState == 0) {
                if (!IsAIHoldingDown && WillAccumulatedLevel >= LevelToCast) CurrentState = 2;
                else {
                    ++ButtonHeldDownTimer;
                    if (WillAccumulatedLevel >= 0 && WillAccumulatedLevel < CurMaxLevels && ButtonHeldDownTimer > Timings[WillAccumulatedLevel] && UpdateAccLevel) {
                        WillIncrementedFlag = true; ++WillAccumulatedLevel;
                    }
                }
            } else CurrentState = CurrentState == 1 ? 2 : 3;
        } else {
            if (CurrentState == 0) {
                Inventory* a = inv;
                long button = -1;
                if ((!a->quickPending && a->buttonSet && (button = 5, g_buttonInQueue)) || (a->quickPending && a->quickInQueue)) {
                    ++ButtonHeldDownTimer;
                    if (WillAccumulatedLevel < CurMaxLevels && ButtonHeldDownTimer > Timings[WillAccumulatedLevel] && UpdateAccLevel) {
                        WillIncrementedFlag = true; ++WillAccumulatedLevel;
                    }
                } else CurrentState = 2;
            } else CurrentState = CurrentState == 1 ? 2 : 3;
        }
        if (!IsAICasting && CurrentState == 2) ++inv->cleared;
    }
};

int main()
{
    static const float timings[3] = { 1.0f, 3.0f, 5.0f };
    Inventory inv = { false, true, false, 0 };
    Timer t = { 0, 0, true, false, 1, 0, timings, 3.0f, false, true, false, false, 2, &inv };
    int ok = 1;
    t.FrameUpdate(); ok = ok && t.ButtonHeldDownTimer == 1 && t.WillAccumulatedLevel == 0 && !t.WillIncrementedFlag;
    t.FrameUpdate(); ok = ok && t.ButtonHeldDownTimer == 2 && t.WillAccumulatedLevel == 1 && t.WillIncrementedFlag;
    t.FrameUpdate(); t.FrameUpdate(); ok = ok && t.WillAccumulatedLevel == 2 && t.CurrentState == 0;
    g_buttonInQueue = false;
    t.FrameUpdate(); ok = ok && t.CurrentState == 2 && inv.cleared == 1;
    t.FrameUpdate(); ok = ok && t.CurrentState == 3 && inv.cleared == 1;
    Timer ai = t; ai.IsAICasting = true; ai.CurrentState = 0; ai.WillAccumulatedLevel = 2; ai.LevelToCast = 2;
    ai.FrameUpdate(); ok = ok && ai.CurrentState == 2;
    ai.CurrentState = 1; ai.FrameUpdate(); ok = ok && ai.CurrentState == 2 && inv.cleared == 1;
    Timer gone = t; gone.creatureAlive = false; gone.WillIncrementedFlag = true; gone.FrameUpdate(); ok = ok && !gone.WillIncrementedFlag;
    if (!ok) { std::printf("008eddd0_TEST FAIL\n"); return 1; }
    std::printf("008eddd0_TEST PASS\n");
    return 0;
}
