// behaviour test for 0x00924930 CCreatureAction_KrakenHighAttackLoop::FinishAction (loop counting, interrupt, next action)
#include <cstdio>
class CThing { public: unsigned char Dead : 1; bool IsDead() const { return Dead; } };
struct Kraken { long loops; int beamEnded; bool headTrack; };
struct Action {
    CThing* target; Kraken* kraken; long LoopNum, FramesRunning, TotalTime; bool Interrupt; int next; long nextLoop;
    bool IsTargetAlive() const { return target && !target->IsDead(); }
    void FinishAction()
    {
        long loops = kraken->loops;
        if (IsTargetAlive() && ++LoopNum < loops) {
            if (FramesRunning < TotalTime) { LoopNum = loops - 2; Interrupt = true; }
            if (LoopNum == loops - 2) { ++kraken->beamEnded; kraken->headTrack = false; }
            next = 1; nextLoop = LoopNum;
        } else
            next = 2;
    }
};
int main()
{
    CThing t = { 0 }, dead = { 1 };
    Kraken k = { 5, 0, true };
    Action a = { &t, &k, 0, 10, 5, false, 0, 0 };
    a.FinishAction();
    bool ok = a.next == 1 && a.LoopNum == 1 && !a.Interrupt && k.beamEnded == 0;
    a.FramesRunning = 1; a.FinishAction();
    ok = ok && a.next == 1 && a.LoopNum == 3 && a.Interrupt && k.beamEnded == 1 && !k.headTrack;
    a.FinishAction(); ok = ok && a.next == 1 && a.LoopNum == 3;
    Action b = { &t, &k, 4, 10, 5, false, 0, 0 }; b.FinishAction(); ok = ok && b.next == 2 && b.LoopNum == 5;
    Action c = { &dead, &k, 0, 10, 5, false, 0, 0 }; c.FinishAction(); ok = ok && c.next == 2 && c.LoopNum == 0;
    std::printf(ok ? "00924930_TEST PASS\n" : "00924930_TEST FAIL\n");
    return ok ? 0 : 1;
}