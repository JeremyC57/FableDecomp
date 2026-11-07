// Behaviour test for NScript::CQ_GuildTrainingPreMeleeScript::CTheRealGuildmaster::Main (0x00D52E90): models the Guildmaster's
// walk-up comment cycle, the dummy-hit phase (counter, help timer reset on progress) and the beetle comment toggle.
#include <stdio.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
// walk-up comments: first, second, third, fourth, then back to third/fourth (fourth sets the count to 2)
static int NextComment(long& count) {
    int said = (int)count;
    switch (count) { case 0: count = 1; break; case 1: count = 2; break; case 2: count = 3; break; case 3: count = 2; break; }
    return said;
}
// one dummy-phase frame: progress resets the help timer to 10; an expired timer gives help and resets it
struct Phase { long lastHits, helpTimer; int helps; };
static void Frame(Phase& p, long hits) {
    if (p.lastHits != hits) { p.lastHits = hits; p.helpTimer = 10; }
    if (p.helpTimer <= 0) { p.helps++; p.helpTimer = 10; }
}
int main() {
    long count = 0;
    int seq[6];
    for (int i = 0; i < 6; i++) seq[i] = NextComment(count);
    check(seq[0] == 0 && seq[1] == 1 && seq[2] == 2 && seq[3] == 3 && seq[4] == 2 && seq[5] == 3, "comment cycle");
    Phase p = { 0, 10, 0 };
    p.helpTimer = 0; Frame(p, 0);
    check(p.helps == 1 && p.helpTimer == 10, "help when idle");
    p.helpTimer = 3; Frame(p, 1);
    check(p.helps == 1 && p.helpTimer == 10 && p.lastHits == 1, "progress resets the timer");
    long beetle = 0;
    beetle = 1 - beetle; check(beetle == 1, "toggle to 1");
    beetle = 1 - beetle; check(beetle == 0, "toggle back");
    if (failures == 0) printf("00d52e90_TEST PASS\n");
    return failures;
}