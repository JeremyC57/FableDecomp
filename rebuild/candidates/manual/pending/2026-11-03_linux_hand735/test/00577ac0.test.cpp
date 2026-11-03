// Behaviour test for CTCInventoryExperience::OpenInventoryInternalPC (0x00577AC0, parked DIFFER): a model of its state
// decisions. A reset open clears both undo maps and plays the top-level tutorial sound only before the first experience
// spending; a non-reset open rebuilds the four slider animators (general, strength, skill, will) with
// remaining = (sum of the four pools) - (available + the three stat experiences), appended in order after an undo
// (CreateBecauseOfUndo) or pushed to the front otherwise, then clears the undo flag and starts the first animator.
#include <cstdio>
#include <deque>
struct SState { bool undoMapsCleared; bool soundPlayed; std::deque<int> animators; float remaining; bool createBecauseOfUndo; bool firstPlays; };
static SState Open(bool reset, bool openedBefore, bool createBecauseOfUndo, const long points[4], long available, const long stat[3])
{
    SState s; s.undoMapsCleared = false; s.soundPlayed = false; s.remaining = 0.0f; s.createBecauseOfUndo = createBecauseOfUndo; s.firstPlays = false;
    if (reset) { s.undoMapsCleared = true; if (!openedBefore) s.soundPlayed = true; return s; }
    s.remaining = (float)(points[0] + points[1] + points[2] + points[3]) - (float)(available + stat[0] + stat[1] + stat[2]);
    for (int i = 0; i < 4; ++i) { if (createBecauseOfUndo) s.animators.push_back(i); else s.animators.push_front(i); }
    s.createBecauseOfUndo = false; s.firstPlays = true;
    return s;
}
int main()
{
    int fails = 0;
    const long points[4] = { 10, 20, 30, 40 }; const long stat[3] = { 5, 6, 7 };
    SState a = Open(true, false, false, points, 0, stat);
    if (!a.undoMapsCleared || !a.soundPlayed || !a.animators.empty()) ++fails;
    SState b = Open(true, true, false, points, 0, stat);
    if (!b.undoMapsCleared || b.soundPlayed) ++fails;
    SState c = Open(false, true, false, points, 2, stat);
    if (c.remaining != 80.0f || c.animators.size() != 4 || c.animators.front() != 3 || c.createBecauseOfUndo || !c.firstPlays) ++fails;
    SState d = Open(false, true, true, points, 2, stat);
    if (d.animators.front() != 0 || d.animators.back() != 3 || d.createBecauseOfUndo) ++fails;
    std::printf(fails ? "00577ac0_TEST FAIL\n" : "00577ac0_TEST PASS\n");
    return fails;
}
