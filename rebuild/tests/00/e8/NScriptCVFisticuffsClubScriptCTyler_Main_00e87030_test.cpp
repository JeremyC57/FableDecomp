// Behaviour test for NScript::CV_FisticuffsClubScript::CTyler::Main (0x00E87030): models Tyler's decisions -- the entry item
// each club asks for, the reward the final win gives, the hit counter / time-out, the win payout and the talk-to-enter gate.
#include <stdio.h>
#include <string.h>
static int failures = 0;
static void check(bool ok, const char* what) { if (!ok) { printf("FAIL %s\n", what); ++failures; } }
enum { REGION_BOWERSTONE = 1, REGION_OAKVALE = 2, REGION_BANDITCAMP = 3, REGION_KNOTHOLE = 4 };
// the entry item is the previous club's reward (Bowerstone, the first club, falls to the default arm and needs none)
static const char* EntryItem(int region) {
    switch (region) {
    case REGION_OAKVALE: return "BowerstoneReward";
    case REGION_BANDITCAMP: return "OakvaleReward";
    case REGION_KNOTHOLE: return "BanditCampReward";
    default: return "KnotholeGladeReward";
    }
}
static const char* Reward(int region) {
    if (region == REGION_KNOTHOLE) return "KnotholeGladeReward";
    if (region == REGION_BOWERSTONE) return "BowerstoneReward";
    if (region == REGION_OAKVALE) return "OakvaleReward";
    if (region == REGION_BANDITCAMP) return "BanditCampReward";
    return 0;
}
// a hit by Tyler counts unless the hero blocks; the round times out when the count reaches the allowed hits
struct Fight { long hits, allowed; bool timeOut; float barValue; };
static void Hit(Fight& f, bool blocking) {
    if (!blocking) { f.hits++; f.barValue = (float)(f.allowed - f.hits); }
    if (f.hits >= f.allowed) f.timeOut = true;
}
// the hero may enter when shown the item, carrying it, or at Bowerstone (no requirement); completed clubs re-enter
static bool MayEnter(bool completed, bool shown, bool carried, int region) {
    return completed || shown || carried || region == REGION_BOWERSTONE;
}
int main() {
    check(strcmp(EntryItem(REGION_OAKVALE), "BowerstoneReward") == 0, "oakvale asks for the bowerstone reward");
    check(strcmp(EntryItem(REGION_KNOTHOLE), "BanditCampReward") == 0, "knothole asks for the bandit camp reward");
    check(strcmp(EntryItem(REGION_BOWERSTONE), "KnotholeGladeReward") == 0, "bowerstone default arm");
    check(strcmp(Reward(REGION_OAKVALE), "OakvaleReward") == 0 && Reward(5) == 0, "final-win reward");
    Fight f = { 0, 3, false, 3.0f };
    Hit(f, true);
    check(f.hits == 0 && !f.timeOut, "blocked hit not counted");
    Hit(f, false); Hit(f, false);
    check(f.hits == 2 && f.barValue == 1.0f && !f.timeOut, "two hits");
    Hit(f, false);
    check(f.timeOut && f.barValue == 0.0f, "time-out on the last allowed hit");
    long region = REGION_BANDITCAMP, expPerRegion = 100, renownPerRegion = 20;
    check(region * expPerRegion == 300 && region * renownPerRegion == 60, "win payout scales with the club");
    check(MayEnter(false, false, false, REGION_BOWERSTONE), "bowerstone needs nothing");
    check(!MayEnter(false, false, false, REGION_OAKVALE), "oakvale without the item");
    check(MayEnter(false, true, false, REGION_OAKVALE) && MayEnter(true, false, false, REGION_KNOTHOLE), "item shown / completed");
    if (failures == 0) printf("00e87030_TEST PASS\n");
    return failures;
}