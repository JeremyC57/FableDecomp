// Behaviour test for CCreatureAction_KrakenRoarLoop::FinishAction (0x00924B90): a model of the row.
#include <stdio.h>

struct Kraken { long roarLoops; int queuedLoop, queuedOut; };
static void Finish(Kraken& k, long& loopNum) { if (++loopNum < k.roarLoops) k.queuedLoop++; else k.queuedOut++; }
static int run()
{
    Kraken k = { 3, 0, 0 }; long loop = 0;
    Finish(k, loop); Finish(k, loop); Finish(k, loop);
    return loop == 3 && k.queuedLoop == 2 && k.queuedOut == 1;
}
int main()
{
    int ok = run();
    printf(ok ? "00924b90_TEST PASS\n" : "00924b90_TEST FAIL\n");
    return ok ? 0 : 1;
}
