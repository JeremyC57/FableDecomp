// Behaviour test for NScript::CQ_HobbeCaveScript::CBanditSacrifice::Main (0x00D973D0): models the bandit's following-comment
// choice, the parting outcome (pay / lost money / keep money / no treasure) and the gold check.
#include <stdio.h>
#include <string.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
static const char* FollowingComment(long& said) {
    switch (++said) { case 1: return "TEXT_QST_036_BANDIT_FOLLOWING_30"; case 2: return "TEXT_QST_036_BANDIT_FOLLOWING_20"; default: return "TEXT_QST_036_BANDIT_FOLLOWING_10"; }
}
enum Outcome { NO_MONEY, GIVE_MONEY, KEEP_MONEY, LOST_AND_KEEP_MONEY };
static Outcome Parting(long gotTreasure, bool takeShare, long heroGold, long demand) {
    if (gotTreasure <= 0) return NO_MONEY;
    if (takeShare && heroGold > demand) return GIVE_MONEY;
    return takeShare ? LOST_AND_KEEP_MONEY : KEEP_MONEY;
}
int main() {
    long said = 0;
    check(strcmp(FollowingComment(said), "TEXT_QST_036_BANDIT_FOLLOWING_30") == 0, "first");
    check(strcmp(FollowingComment(said), "TEXT_QST_036_BANDIT_FOLLOWING_20") == 0, "second");
    check(strcmp(FollowingComment(said), "TEXT_QST_036_BANDIT_FOLLOWING_10") == 0 && said == 3, "later");
    check(Parting(0, true, 1000, 100) == NO_MONEY, "no treasure");
    check(Parting(1, true, 1000, 100) == GIVE_MONEY, "pays");
    check(Parting(1, true, 100, 100) == LOST_AND_KEEP_MONEY, "too poor");
    check(Parting(2, false, 1000, 100) == KEEP_MONEY, "refused");
    if (failures == 0) printf("00d973d0_TEST PASS\n");
    return failures;
}
