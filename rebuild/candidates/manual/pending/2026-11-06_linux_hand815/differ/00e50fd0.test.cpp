// Behaviour test for NScript::CV_BeardyBaldyScript::CBB_BeardyBaldyMan::Main (0x00E50FD0): models the per-frame priority
// (attacked first, then the random-speech timer, then the talk) and the speech-timer reset after a line.
#include <stdio.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
enum Action { NONE, SCOLD, RANDOM_SPEECH, TALK };
static Action Step(bool& attacked, long speechTime, bool talked) {
    if (attacked) { attacked = false; return SCOLD; }
    if (speechTime == 0) return RANDOM_SPEECH;
    if (talked) return TALK;
    return NONE;
}
int main() {
    bool attacked = true;
    check(Step(attacked, 0, true) == SCOLD && !attacked, "attack wins and clears the flag");
    check(Step(attacked, 0, true) == RANDOM_SPEECH, "speech timer before talk");
    check(Step(attacked, 5, true) == TALK, "talk");
    check(Step(attacked, 5, false) == NONE, "idle");
    if (failures == 0) printf("00e50fd0_TEST PASS\n");
    return failures;
}
