// Behaviour test for CTCInventoryExperience::AddStatToList (0x005729D8): a model of the entry builder's decisions. The sound
// name gets the stat's suffix; a stat with no possible level-up and no undo record is made unavailable (its arrow deleted)
// and gets no sub list; otherwise the sub list holds a buy entry when levels can be bought and an undo entry when an undo
// record exists (InitialiseOffsets 2 only when both); the buy spends min(category pool, cost) from the category slider and
// the rest of the cost from the general slider.
#include <cstdio>
#include <cstring>
#include <string>
struct SResult { std::string sound; bool unavailable; unsigned long offsets; int entries; long fromCategory; long fromGeneral; };
static SResult AddStat(int stat, long possible, bool hasUndo, long cost, long categoryExperience)
{
    SResult r; r.sound = "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_";
    if (stat == 2) r.sound += "TOUGHNESS";
    if (stat == 0) r.sound += "PHYSIQUE";
    if (stat == 1) r.sound += "HEALTH";
    if (stat == 3) r.sound += "SPEED";
    if (stat == 4) r.sound += "ACCURACY";
    if (stat == 5) r.sound += "GUILE";
    if (stat == 9) r.sound += "MAGICPOWER";
    r.unavailable = false; r.offsets = 0; r.entries = 0; r.fromCategory = 0; r.fromGeneral = 0;
    if (possible <= 0 && !hasUndo) { r.unavailable = true; return r; }
    r.offsets = 1;
    if (possible > 0 && hasUndo) r.offsets = 2;
    if (possible > 0) {
        ++r.entries;
        if (categoryExperience > 0) r.fromCategory = categoryExperience <= cost ? categoryExperience : cost;
        cost -= categoryExperience;
        if (cost > 0) r.fromGeneral = cost;
    }
    if (hasUndo) ++r.entries;
    return r;
}
int main()
{
    int fails = 0;
    SResult a = AddStat(2, 1, false, 100, 30);
    if (a.sound != "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_TOUGHNESS" || a.unavailable || a.offsets != 1 || a.entries != 1 || a.fromCategory != 30 || a.fromGeneral != 70) ++fails;
    SResult b = AddStat(9, 0, false, 100, 30);
    if (b.sound != "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_MAGICPOWER" || !b.unavailable || b.entries != 0) ++fails;
    SResult c = AddStat(0, 2, true, 50, 80);
    if (c.sound != "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_PHYSIQUE" || c.offsets != 2 || c.entries != 2 || c.fromCategory != 50 || c.fromGeneral != 0) ++fails;
    SResult d = AddStat(7, 0, true, 50, 0);
    if (d.sound != "SND_TEXT_QST_TUTORIAL_GUI_EXPSPEND_BUY_" || d.unavailable || d.offsets != 1 || d.entries != 1) ++fails;
    std::printf(fails ? "005729d8_TEST FAIL\n" : "005729d8_TEST PASS\n");
    return fails;
}
