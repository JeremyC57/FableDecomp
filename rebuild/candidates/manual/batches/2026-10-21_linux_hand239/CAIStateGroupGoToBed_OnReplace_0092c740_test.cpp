#include <cstdio>
// Behaviour copy of CAIStateGroup_GoToBed::OnReplace (0x0092C740).
struct Thing { bool dead; int pointsFreed; long lastFreed; Thing* occupant; bool hasPoints, hasBed; };
static bool Alive(Thing* t) { return t && !t->dead; }
struct GoToBed {
    Thing* PBed; Thing* creature; bool IsBedSleepPointMarker; long ActionPointIndex;
    void OnReplace() {
        if (!IsBedSleepPointMarker) {
            if (ActionPointIndex != -1 && Alive(PBed)) { Thing* points = PBed; ++points->pointsFreed; points->lastFreed = ActionPointIndex; ActionPointIndex = -1; }
        } else if (Alive(PBed)) {
            Thing* bed = PBed; Thing* c = creature; if (bed->occupant == c) bed->occupant = 0;
        }
        PBed = 0;
    }
};
int main() {
    Thing me = {false, 0, 0, 0, false, false};
    Thing bed = {false, 0, 0, &me, true, true};
    GoToBed a = { &bed, &me, false, 3 }; a.OnReplace();
    if (bed.pointsFreed != 1 || bed.lastFreed != 3 || a.ActionPointIndex != -1 || a.PBed != 0) { std::printf("0092c740_TEST FAIL\n"); return 1; }
    GoToBed b = { &bed, &me, true, 5 }; b.OnReplace();
    if (bed.occupant != 0 || b.ActionPointIndex != 5 || b.PBed != 0) { std::printf("0092c740_TEST FAIL\n"); return 1; }
    Thing other = {false, 0, 0, 0, false, false}; bed.occupant = &other;
    GoToBed c = { &bed, &me, true, 5 }; c.OnReplace();
    bed.dead = true; GoToBed d = { &bed, &me, false, 2 }; d.OnReplace();
    if (bed.occupant != &other || d.ActionPointIndex != 2 || bed.pointsFreed != 1) { std::printf("0092c740_TEST FAIL\n"); return 1; }
    std::printf("0092c740_TEST PASS\n"); return 0;
}
