// Behaviour test for CTCFishing::FinishFishing (007b0950): sound stops, GUI reset, scoring of a caught fish, the finish message and the stage reset
#include <cstdio>

struct Fishing { unsigned long reelSound, escapeSound; long stage; bool stageInit; };
struct Log { int soundsStopped, scores, messages, modeRemoved; float lastScore; bool meter; bool msgHasFish; };

static void FinishFishing(Fishing& f, Log& log, bool heroAlive, bool heroHasSound, bool caught, float fishWeight)
{
    if (!heroAlive)
        return;
    unsigned long reel = f.reelSound;
    if (reel && heroHasSound) {
        ++log.soundsStopped;
        f.reelSound = 0;
    }
    unsigned long escape = f.escapeSound;
    if (escape && heroHasSound) {
        ++log.soundsStopped;
        f.escapeSound = 0;
    }
    log.meter = false;
    bool fish = false;
    if (caught) {
        fish = true;
        ++log.scores;
        log.lastScore = fishWeight;
    }
    ++log.messages;
    log.msgHasFish = fish;
    if (f.stage) {
        f.stage = 0;
        f.stageInit = false;
    }
    ++log.modeRemoved;
}

int main()
{
    int fails = 0;
    Fishing f = { 11, 0, 3, true };
    Log log = { 0, 0, 0, 0, 0.0f, true, false };
    FinishFishing(f, log, false, true, true, 2.5f);
    if (log.messages != 0 || f.reelSound != 11) ++fails;
    FinishFishing(f, log, true, true, true, 2.5f);
    if (f.reelSound != 0 || log.soundsStopped != 1 || log.scores != 1 || log.lastScore != 2.5f || !log.msgHasFish
        || f.stage != 0 || f.stageInit || log.meter || log.modeRemoved != 1) ++fails;
    f.escapeSound = 7;
    f.stage = 0;
    f.stageInit = true;
    FinishFishing(f, log, true, false, false, 9.0f);
    if (f.escapeSound != 7 || log.scores != 1 || log.msgHasFish || !f.stageInit || log.messages != 2) ++fails;
    if (fails) {
        printf("007b0950_TEST FAIL\n");
        return 1;
    }
    printf("007b0950_TEST PASS\n");
    return 0;
}
