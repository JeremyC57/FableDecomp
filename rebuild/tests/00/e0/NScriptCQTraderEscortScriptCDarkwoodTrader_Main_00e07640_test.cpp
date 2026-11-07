// Behaviour test for NScript::CQ_TraderEscortScript::CDarkwoodTrader::Main (0x00E07640): models the follower's per-frame
// bookkeeping -- the incubation comment ladder, the end-walk health top-up, the attack comment timers and the banter schedule.
#include <stdio.h>
#include <stdlib.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
enum { INFECTED_NONE, INFECTED_LOW, INFECTED_MEDIUM, INFECTED_HIGH };
// the ladder: the first band whose time threshold is passed and whose level is not yet reached comments; a spoken comment raises it
static int Incubation(long left, long high, long medium, long low, int infected, bool spoken) {
    if (left < high && infected < INFECTED_HIGH) return spoken ? INFECTED_HIGH : infected;
    if (left < medium && infected < INFECTED_MEDIUM) return spoken ? INFECTED_MEDIUM : infected;
    if (left < low && infected < INFECTED_LOW) return spoken ? INFECTED_LOW : infected;
    return infected;
}
// end walk: ModifyThingHealth(Me, 30 - health) leaves the trader on exactly 30
static float EndHealth(float health) { return health + (30.0f - health); }
// banter: a spoken banter waits 30..49 frames and counts, a refused one waits 25..39
static long NextBanter(bool spoken, int r, long& times) {
    if (spoken) { ++times; return r % 20 + 30; }
    return r % 15 + 25;
}
// hit comments: not by the hero (or healed by the hero) -> UNDER_ATTACK, timer 3; by the hero -> HERO_HIT_ME, timer 2
static long HitComment(float health, bool byHero, bool heroSpecial, bool heroHeal) {
    if (health > 5.0f && !byHero && (!heroSpecial || heroHeal)) return 3;
    if (health > 5.0f && (byHero || (heroSpecial && !heroHeal))) return 2;
    return 0;
}
int main() {
    check(Incubation(5, 10, 20, 30, INFECTED_NONE, true) == INFECTED_HIGH, "high band first");
    check(Incubation(15, 10, 20, 30, INFECTED_NONE, true) == INFECTED_MEDIUM, "medium band");
    check(Incubation(15, 10, 20, 30, INFECTED_MEDIUM, true) == INFECTED_MEDIUM, "medium already said, low skipped");
    check(Incubation(25, 10, 20, 30, INFECTED_NONE, false) == INFECTED_NONE, "comment refused keeps the level");
    check(Incubation(40, 10, 20, 30, INFECTED_NONE, true) == INFECTED_NONE, "nothing before the low band");
    check(EndHealth(12.5f) == 30.0f && EndHealth(80.0f) == 30.0f, "end health 30");
    long times = 0;
    long w = NextBanter(true, 47, times);
    check(w == 37 && times == 1, "spoken banter");
    w = NextBanter(false, 47, times);
    check(w == 27 && times == 1, "refused banter");
    check(HitComment(50.0f, false, false, false) == 3, "under attack");
    check(HitComment(50.0f, true, false, false) == 2, "hero hit me");
    check(HitComment(50.0f, false, true, true) == 3, "hero heal spell is not an attack by the hero");
    check(HitComment(4.0f, true, false, false) == 0, "too hurt to comment");
    if (failures == 0) printf("00e07640_TEST PASS\n");
    return failures;
}