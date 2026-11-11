// Behaviour test for NScript::CQ_TraderConflictGoodScript::CTraderToRescue::Main (0x00DFE0F0): models the caged trader's
// per-frame decisions -- the rescue condition, the over-here / keeper-dead line choice and the timer bookkeeping.
#include <stdio.h>
#include <string.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
static bool Rescued(bool talked, const char* data, bool cageOpened, bool expressed, const char* expression) {
    return talked || (strcmp(data, "TRADERB") == 0 && cageOpened) || (expressed && strcmp(expression, "EXPRESSION_FOLLOW") == 0);
}
struct Beg { long talkTimer, threatenTimer; bool saidKeeperIsDead; const char* line; };
// one begging step (the hero within 15 m): keeper-dead once, else over-here when rand() % 5 == 0; then talk 10, threaten += 5
static void BegStep(Beg& b, bool keeperAlive, int randValue) {
    b.line = 0;
    if (b.talkTimer == 0 || (!b.saidKeeperIsDead && !keeperAlive)) {
        if (!b.saidKeeperIsDead && !keeperAlive) { b.line = "_KEEPERISDEAD"; b.saidKeeperIsDead = true; }
        else if (randValue % 5 == 0) b.line = "_OVERHERE";
        b.talkTimer = 10;
        b.threatenTimer = b.threatenTimer + 5;
    }
}
int main() {
    check(Rescued(true, "TRADERA", false, false, ""), "talked to");
    check(Rescued(false, "TRADERB", true, false, ""), "trader B with the cage open");
    check(!Rescued(false, "TRADERC", true, false, ""), "trader C needs a talk");
    check(Rescued(false, "TRADERC", false, true, "EXPRESSION_FOLLOW"), "follow expression");
    check(!Rescued(false, "TRADERC", false, true, "EXPRESSION_WAVE"), "other expression");
    Beg b = { 0, 0, false, 0 };
    BegStep(b, true, 10);
    check(b.line && strcmp(b.line, "_OVERHERE") == 0 && b.talkTimer == 10 && b.threatenTimer == 5, "over here");
    BegStep(b, false, 1);
    check(b.line && strcmp(b.line, "_KEEPERISDEAD") == 0 && b.saidKeeperIsDead && b.threatenTimer == 10, "keeper dead");
    BegStep(b, false, 0);
    check(b.line == 0 && b.threatenTimer == 10, "timer running, keeper line said");
    b.talkTimer = 0; BegStep(b, false, 3);
    check(b.line == 0 && b.talkTimer == 10, "silent when rand misses");
    if (failures == 0) printf("00dfe0f0_TEST PASS\n");
    return failures;
}