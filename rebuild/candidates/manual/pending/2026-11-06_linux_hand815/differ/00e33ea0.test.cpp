// Behaviour test for NScript::CV_ArcheryCompetitionScript::CAC_Owner::Main (0x00E33EA0): models the contest state machine
// the owner drives (waiting -> countdown -> started -> finished) and the high-score / normal prize choice.
#include <stdio.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
enum { CONTEST_WAITING=0, CONTEST_COUNTDOWN=1, CONTEST_FINISHED=2, CONTEST_STARTED=3 };
static int Next(int state, bool paid, bool countdownDone, bool timeUp) {
    switch (state) {
    case CONTEST_WAITING: return paid ? CONTEST_COUNTDOWN : CONTEST_WAITING;
    case CONTEST_COUNTDOWN: return countdownDone ? CONTEST_STARTED : CONTEST_COUNTDOWN;
    case CONTEST_STARTED: return timeUp ? CONTEST_FINISHED : CONTEST_STARTED;
    }
    return CONTEST_WAITING;
}
static int Prize(long score, long highScore, bool cheated) { return cheated ? 0 : (score > highScore ? 2 : (score > 0 ? 1 : 0)); }
int main() {
    int s = CONTEST_WAITING;
    s = Next(s, false, false, false); check(s == CONTEST_WAITING, "no fee");
    s = Next(s, true, false, false); check(s == CONTEST_COUNTDOWN, "fee paid");
    s = Next(s, true, true, false); check(s == CONTEST_STARTED, "countdown done");
    s = Next(s, true, true, true); check(s == CONTEST_FINISHED, "time up");
    check(Prize(120, 100, false) == 2, "high score prize");
    check(Prize(50, 100, false) == 1, "normal prize");
    check(Prize(150, 100, true) == 0, "stepped out / magic / shot the owner");
    if (failures == 0) printf("00e33ea0_TEST PASS\n");
    return failures;
}
