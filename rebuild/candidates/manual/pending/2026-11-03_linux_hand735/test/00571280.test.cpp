// Behaviour test for CTCInventoryExperience::AddAbilityToListPC (0x00571280, parked DIFFER; the PC twin of 0x00570332): a model of the entry builder's
// decisions. The name gains " +<level+2>" while the ability is below its maximum; the morality slider colour is red below
// -DELTA_NOTIONAL_ZERO, blue above it, absent in between; an ability with no possible level-up and no undo record is made
// unavailable; otherwise the sub list holds a buy entry when levels can be bought and an undo entry when an undo record exists
// (InitialiseOffsets 2 only when both); the buy spends min(will pool, cost) from the will slider and the rest from the general one.
#include <cstdio>
#include <string>
struct SResult { std::string suffix; int morality; bool unavailable; unsigned long offsets; int entries; long fromWill; long fromGeneral; };
static SResult AddAbility(long level, long maxLevel, float moralityFactor, long possible, bool hasUndo, long cost, long willExperience)
{
    SResult r; r.morality = 0; r.unavailable = false; r.offsets = 0; r.entries = 0; r.fromWill = 0; r.fromGeneral = 0;
    if (level < maxLevel) { char b[32]; std::sprintf(b, " +%ld", level + 2); r.suffix = b; }
    const float eps = 0.0001f;
    if (moralityFactor < -eps) r.morality = -1; else if (moralityFactor > eps) r.morality = 1;
    if (possible <= 0 && !hasUndo) { r.unavailable = true; return r; }
    r.offsets = (possible > 0 && hasUndo) ? 2 : 1;
    if (possible > 0) {
        ++r.entries;
        if (willExperience > 0) r.fromWill = willExperience <= cost ? willExperience : cost;
        cost -= willExperience;
        if (cost > 0) r.fromGeneral = cost;
    }
    if (hasUndo) ++r.entries;
    return r;
}
int main()
{
    int fails = 0;
    SResult a = AddAbility(1, 4, -0.5f, 1, false, 100, 30);
    if (a.suffix != " +3" || a.morality != -1 || a.unavailable || a.offsets != 1 || a.entries != 1 || a.fromWill != 30 || a.fromGeneral != 70) ++fails;
    SResult b = AddAbility(4, 4, 0.0f, 0, false, 100, 30);
    if (b.suffix != "" || b.morality != 0 || !b.unavailable || b.entries != 0) ++fails;
    SResult c = AddAbility(0, 4, 0.5f, 2, true, 50, 80);
    if (c.suffix != " +2" || c.morality != 1 || c.offsets != 2 || c.entries != 2 || c.fromWill != 50 || c.fromGeneral != 0) ++fails;
    std::printf(fails ? "00571280_TEST FAIL\n" : "00571280_TEST PASS\n");
    return fails;
}
