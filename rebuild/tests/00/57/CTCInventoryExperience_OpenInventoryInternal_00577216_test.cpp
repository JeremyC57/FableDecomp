// Behaviour test for CTCInventoryExperience::OpenInventoryInternal (0x00577216): a model of the console screen opening. A fresh
// open (reset) clears both undo maps, plays the top-level tutorial once (until the screen has been used) and puts the menu in
// state 5; a re-open after a purchase builds four count-down animators whose shared "overall" is the points snapshot minus the
// current experience, appended in stat order after an undo and prepended (reverse order) otherwise, and plays the first one.
#include <cstdio>
#include <list>
struct SAnim { int stat; float start, end, overall; bool play; };
struct SScreen {
    bool openedBefore, undo; int undoEntries, tutorials, menuState;
    long points[4], current[4];
    std::list<SAnim> animators;
};
static void Open(SScreen& s, bool reset)
{
    if (reset) {
        s.undoEntries = 0;
        if (!s.openedBefore) ++s.tutorials;
    } else {
        float overall = (float)(s.points[0] + s.points[1] + s.points[2] + s.points[3]);
        overall -= (float)(s.current[0] + s.current[1] + s.current[2] + s.current[3]);
        s.animators.clear();
        for (int i = 0; i < 4; ++i) {
            SAnim a; a.stat = i; a.start = (float)s.points[i]; a.end = (float)s.current[i]; a.overall = overall; a.play = false;
            if (s.undo) s.animators.push_back(a); else s.animators.push_front(a);
        }
        s.undo = false;
        s.animators.front().play = true;
    }
    if (reset) s.menuState = 5;
}
int main()
{
    int fails = 0;
    SScreen s; s.openedBefore = false; s.undo = false; s.undoEntries = 3; s.tutorials = 0; s.menuState = 0;
    for (int i = 0; i < 4; ++i) { s.points[i] = 100 * (i + 1); s.current[i] = 90 * (i + 1); }
    Open(s, true);
    if (s.undoEntries != 0 || s.tutorials != 1 || s.menuState != 5 || !s.animators.empty()) ++fails;
    s.menuState = 0;
    Open(s, false);
    if (s.animators.size() != 4 || s.animators.front().stat != 3 || !s.animators.front().play || s.menuState != 0) ++fails;
    if (s.animators.front().overall < 99.9f || s.animators.front().overall > 100.1f) ++fails;
    s.undo = true;
    Open(s, false);
    if (s.animators.front().stat != 0 || !s.animators.front().play || s.undo) ++fails;
    std::printf(fails ? "00577216_TEST FAIL\n" : "00577216_TEST PASS\n");
    return fails;
}