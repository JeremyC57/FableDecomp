// behaviour test for 0x00924520 CCreatureAction_KrakenLowAttackLoop::FinishAction
extern "C" int printf(const char*, ...);
static int g_fail;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static int g_beamEnds, g_nextLoop, g_outOf, g_baseFinish; static long g_queuedLoop; static bool g_queuedInterrupt;
struct Action {
    bool TargetAlive; long FramesRunning, TotalTime; bool Interrupt; long LoopNum; long AttackLoops;
    void FinishAction()
    {
        ++g_baseFinish;
        long attack_loops = AttackLoops;
        if (TargetAlive && ++LoopNum < attack_loops) {
            if (FramesRunning < TotalTime) { LoopNum = attack_loops - 2; Interrupt = true; }
            if (LoopNum == attack_loops - 2) ++g_beamEnds;
            ++g_nextLoop; g_queuedLoop = LoopNum; g_queuedInterrupt = Interrupt;
        } else
            ++g_outOf;
    }
};
int main()
{
    Action a = { true, 10, 10, false, 0, 5 };
    a.FinishAction(); CHECK(g_nextLoop == 1 && g_queuedLoop == 1 && !g_queuedInterrupt && g_beamEnds == 0);
    a.FramesRunning = 3; a.FinishAction(); CHECK(g_queuedLoop == 3 && g_queuedInterrupt && g_beamEnds == 1);
    a.FramesRunning = 10; a.FinishAction(); CHECK(g_queuedLoop == 4 && g_beamEnds == 1);
    a.FinishAction(); CHECK(g_outOf == 1 && a.LoopNum == 5);
    Action b = { false, 0, 0, false, 0, 5 }; b.FinishAction(); CHECK(g_outOf == 2 && b.LoopNum == 0 && g_baseFinish == 5);
    if (g_fail == 0) printf("00924520_TEST PASS\n");
    return g_fail;
}