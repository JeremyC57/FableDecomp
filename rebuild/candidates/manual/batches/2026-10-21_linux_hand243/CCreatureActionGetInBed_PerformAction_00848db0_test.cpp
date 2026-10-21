#include <cstdio>
// Behaviour copy of CCreatureAction_GetInBed::PerformAction (0x00848DB0): push a sleep mode, then start the bed animation or mark the bed occupied.
struct Bed { bool Empty; long getIn, sleep; bool started; void StartGetInBedAnimation(long a, long b) { getIn = a; sleep = b; started = true; } };
struct Thing { bool alive; bool hasBed; Bed bed; };
struct ModeManager { int modes; const char* lastGroup; void AddMode(const char* group) { ++modes; lastGroup = group; } };
struct Action {
    ModeManager* mgr; Thing* bedThing; const char* AnimGroup; long BedGetInAnim, BedSleepAnim;
    void PerformAction() {
        mgr->AddMode(AnimGroup);
        if (bedThing && bedThing->alive && bedThing->hasBed) {
            Bed* b = &bedThing->bed;
            if (BedGetInAnim) b->StartGetInBedAnimation(BedGetInAnim, BedSleepAnim); else b->Empty = false;
        }
    }
};
int main()
{
    ModeManager m = { 0, 0 }; Thing bed = { true, true, { true, 0, 0, false } };
    Action a = { &m, &bed, "SLEEP", 7, 9 };
    a.PerformAction();
    bool ok = m.modes == 1 && m.lastGroup == a.AnimGroup && bed.bed.started && bed.bed.getIn == 7 && bed.bed.sleep == 9 && bed.bed.Empty;
    a.BedGetInAnim = 0; a.PerformAction();
    ok = ok && !bed.bed.Empty && m.modes == 2;
    Thing dead = { false, true, { true, 0, 0, false } }; a.bedThing = &dead; a.PerformAction();
    ok = ok && dead.bed.Empty && m.modes == 3;
    std::printf(ok ? "00848db0_TEST PASS\n" : "00848db0_TEST FAIL\n");
    return ok ? 0 : 1;
}
