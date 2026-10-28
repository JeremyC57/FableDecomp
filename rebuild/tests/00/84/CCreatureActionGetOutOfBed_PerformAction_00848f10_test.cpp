// behaviour test for 0x00848F10 CCreatureAction_GetOutOfBed::PerformAction(): drop mode 13, mark a live bed's CTCBed
// empty, and always flag the action performed
#include <cstdio>
struct Bed { bool Empty; };
struct Modes { unsigned long lastRemoved; int count; void RemoveMode(unsigned long m) { lastRemoved = m; ++count; } };
struct Thing { bool dead; Bed* bed; bool IsDead() const { return dead; } bool GetTC(Bed** out) { if (!bed) return false; *out = bed; return true; } };
struct Action {
    Modes* modes; Thing* PBed; bool PerformedAction;
    void PerformAction()
    {
        modes->RemoveMode(13);
        if (PBed && !PBed->IsDead()) {
            Bed* bed;
            if (PBed->GetTC(&bed))
                bed->Empty = true;
        }
        PerformedAction = true;
    }
};
int main()
{
    int fails = 0;
    Modes m = { 0, 0 };
    Bed b = { false };
    Thing bedThing = { false, &b };
    Action a = { &m, &bedThing, false };
    a.PerformAction();
    if (!b.Empty || !a.PerformedAction || m.lastRemoved != 13 || m.count != 1) fails |= 1;
    b.Empty = false; bedThing.dead = true; a.PerformedAction = false; a.PerformAction();
    if (b.Empty || !a.PerformedAction || m.count != 2) fails |= 2;
    a.PBed = 0; a.PerformedAction = false; a.PerformAction();
    if (!a.PerformedAction || m.count != 3) fails |= 4;
    if (fails) { std::printf("00848f10_TEST FAIL %d\n", fails); return 1; }
    std::printf("00848f10_TEST PASS\n");
    return 0;
}