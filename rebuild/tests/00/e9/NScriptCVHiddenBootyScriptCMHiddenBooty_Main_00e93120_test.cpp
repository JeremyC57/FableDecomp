// Behaviour test for NScript::CV_HiddenBootyScript::CM_HiddenBooty::Main (0x00E93120): the any-clue / all-clue tests (|| and &&
// over the six clues), the re-check only on a received item while not all clues are held, the fake booty added once, swapped for
// the real booty (fake removed first) once all clues are held, and quest completion when the hero is rewarded from this spot.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int frame, termFrame, rewardFrame, itemFrame, clueFrame[6];
    bool hasAll, hasAny, questComplete, fakeAdded, realAdded;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Has(int i) const { return frame >= clueFrame[i]; }
    bool Any() const { return Has(0) || Has(1) || Has(2) || Has(3) || Has(4) || Has(5); }
    bool All() const { return Has(0) && Has(1) && Has(2) && Has(3) && Has(4) && Has(5); }
    void Main()
    {
        hasAny = Any(); hasAll = All();
        while (!questComplete) {
            ++frame;
            if (Term()) return;
            if (!hasAll && frame >= itemFrame) { if (Term()) return; hasAny = Any(); hasAll = All(); }
            if (hasAny && !fakeAdded) { if (Term()) return; Ev("ADD_FAKE"); fakeAdded = true; }
            if (hasAll && !realAdded) {
                if (Term()) return;
                if (fakeAdded) { if (Term()) return; Ev("REMOVE_FAKE"); }
                Ev("ADD_REAL"); realAdded = true;
            }
            if (frame == rewardFrame) { if (Term()) return; Ev("COMPLETE"); questComplete = true; }
        }
    }
};
static void Make(Sim& s) { s.log[0] = 0; s.frame = 0; s.termFrame = 100; s.rewardFrame = 10; s.itemFrame = 0;
    for (int i = 0; i < 6; ++i) s.clueFrame[i] = 1000; s.hasAll = s.hasAny = s.questComplete = s.fakeAdded = s.realAdded = false; }
int main()
{
    Sim a; Make(a); a.clueFrame[2] = 2; for (int i = 0; i < 6; ++i) if (a.clueFrame[i] > 5) a.clueFrame[i] = 5; a.Main();
    bool ok = std::strcmp(a.log, "ADD_FAKE;REMOVE_FAKE;ADD_REAL;COMPLETE;") == 0 && a.questComplete && a.frame == 10;
    Sim b; Make(b); b.clueFrame[4] = 0; b.itemFrame = 50; b.Main();
    ok = ok && std::strcmp(b.log, "ADD_FAKE;COMPLETE;") == 0 && !b.realAdded;
    Sim c; Make(c); for (int i = 0; i < 6; ++i) c.clueFrame[i] = 0; c.termFrame = 3; c.rewardFrame = 5; c.Main();
    ok = ok && std::strcmp(c.log, "ADD_FAKE;REMOVE_FAKE;ADD_REAL;") == 0 && !c.questComplete;
    Sim d; Make(d); d.questComplete = true; d.Main();
    ok = ok && d.log[0] == 0 && d.frame == 0;
    std::printf(ok ? "00e93120_TEST PASS\n" : "00e93120_TEST FAIL\n");
    return ok ? 0 : 1;
}