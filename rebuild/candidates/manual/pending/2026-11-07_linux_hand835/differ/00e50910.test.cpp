// Behaviour test for NScript::CV_BeardyBaldyScript::WatchForQuestCardConditions 0x00E50910 (parked): models the script loop over a
// scripted sequence of frames -- the quest card and objective 02 once Phase 1 completes, objective 03 + GoTalkToBeardyBaldy and the
// acquired flags reset on a new hair item, the BowerstoneSlumsWarehouses region wait, objective 02 again after the hero talks to a
// living Beardy Baldy (not when Phase 3 is complete), GoTalkToBarber at the end of each pass, and termination.
#include <stdio.h>
#include <string.h>
struct Script {
    int Frame, TerminateAt; bool Phase1, Phase3, Hairdo, Beard, Tash, Alive;
    int Cards, Obj02, Obj03, Barber, Beardy;
    bool Term() const { return Frame >= TerminateAt; }
    void NewFrame() { ++Frame; if (Frame == 2) Phase1 = true; if (Frame == 4) Hairdo = true; }
    const char* RegionLoaded() const { return Frame >= 6 ? "BowerstoneSlumsWarehouses" : (Frame == 5 ? "Elsewhere" : 0); }
    bool TalkedTo() const { return Frame >= 7; }
};
static void Run(Script& s)
{
    while (!s.Phase1) { s.NewFrame(); if (s.Term()) return; }
    if (s.Term()) return;
    ++s.Cards; ++s.Obj02; ++s.Barber;
    for (;;) {
        if (s.Term()) return;
        while (!s.Hairdo && !s.Beard && !s.Tash) { s.NewFrame(); if (s.Term()) return; }
        if (s.Term()) return;
        ++s.Obj03; ++s.Beardy; s.Hairdo = s.Beard = s.Tash = false;
        for (;;) {
            if (s.Term()) return;
            while (!s.RegionLoaded()) { s.NewFrame(); if (s.Term()) return; }
            if (s.Term()) return;
            if (strcmp(s.RegionLoaded(), "BowerstoneSlumsWarehouses") == 0) break;
            s.NewFrame();
        }
        if (s.Alive) {
            while (!s.TalkedTo()) { s.NewFrame(); if (s.Term()) return; }
            if (!s.Phase3) ++s.Obj02;
        }
        ++s.Barber; s.NewFrame();
    }
}
int main()
{
    Script a = { 0, 9, false, false, false, false, false, true, 0, 0, 0, 0, 0 };
    Run(a);
    Script b = { 0, 9, false, true, false, false, false, true, 0, 0, 0, 0, 0 };
    Run(b);
    bool ok = a.Cards == 1 && a.Obj02 == 2 && a.Obj03 == 1 && a.Beardy == 1 && a.Barber == 2 && !a.Hairdo
           && b.Obj02 == 1 && b.Barber == 2;
    printf(ok ? "00e50910_TEST PASS\n" : "00e50910_TEST FAIL\n");
    return ok ? 0 : 1;
}
