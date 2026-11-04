// Behaviour test for 0x008CF7D0 CCreatureAction_TentacleStrikeLoop::FinishAction: a portable model of the retail decision
// (not interrupted and the elapsed frames within SmackDownTime * fps -> another strike loop with the same start tick;
// otherwise strike-out-of).
#include <cstdio>

enum Next { LOOP, OUT_OF };
static Next FinishAction(bool interrupt, long fps, float smackDownSecs, long now, long start, long* nextStart) {
    if (!interrupt && now - start <= (long)(smackDownSecs * fps)) { *nextStart = start; return LOOP; }
    return OUT_OF;
}
int main() {
    long next = -1;
    bool ok = FinishAction(false, 30, 2.0f, 100, 50, &next) == LOOP && next == 50;
    ok = ok && FinishAction(false, 30, 2.0f, 110, 50, &next) == LOOP;            // exactly 60 frames: still looping
    ok = ok && FinishAction(false, 30, 2.0f, 111, 50, &next) == OUT_OF;
    ok = ok && FinishAction(false, 30, 2.0f, 200, 50, &next) == OUT_OF;
    ok = ok && FinishAction(true, 30, 2.0f, 60, 50, &next) == OUT_OF;
    ok = ok && FinishAction(false, 30, 0.5f, 64, 50, &next) == LOOP && FinishAction(false, 30, 0.5f, 66, 50, &next) == OUT_OF;
    std::printf(ok ? "008cf7d0_TEST PASS\n" : "008cf7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}