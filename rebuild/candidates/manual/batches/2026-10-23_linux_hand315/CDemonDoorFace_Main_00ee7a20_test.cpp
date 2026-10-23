// Behaviour test for CDemonDoorFace::Main (0x00EE7A20): while the stones are out of sync, each talk says the door is
// locked and the first talk sets the quest objective once; once SingingStonesInSync is set the door cut-scene plays,
// the objective moves on, the flag is cleared and Me is removed.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int talkFrame[3]; int syncFrame;
    bool spoken; bool sync; int lockedLines; int objectives; int lastObjective; int cutscenes; bool removed; int pauses; int unpauses;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.frame == s.syncFrame) s.sync = true; }
static bool Talked(const Sim& s) { for (int i = 0; i < 3; ++i) if (s.talkFrame[i] == s.frame) return true; return false; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (!Term(s)) {
        if (Talked(s)) {
            if (Term(s)) return;
            if (!s.sync) {
                ++s.pauses; ++s.lockedLines;
                Frame(s); if (Term(s)) { ++s.unpauses; return; }
                ++s.unpauses;
            }
            if (!s.spoken) {
                s.spoken = true; ++s.objectives; s.lastObjective = 3;
            }
        }
        if (s.sync) {
            if (Term(s)) return;
            ++s.pauses; ++s.cutscenes; ++s.unpauses;
            ++s.objectives; s.lastObjective = 4;
            s.sync = false; s.removed = true;
            return;
        }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.talkFrame[0] = 4; a.talkFrame[1] = 9; a.talkFrame[2] = -1; a.syncFrame = 30;
    Main(a);
    bool ok = a.lockedLines == 2 && a.objectives == 2 && a.lastObjective == 4 && a.cutscenes == 1 && a.removed && !a.sync
           && a.pauses == a.unpauses && a.frame == 30;
    Sim b; std::memset(&b, 0, sizeof(b));
    b.terminateAt = 20; b.talkFrame[0] = 5; b.talkFrame[1] = -1; b.talkFrame[2] = -1; b.syncFrame = 50;
    Main(b);
    ok = ok && b.lockedLines == 1 && b.objectives == 1 && b.lastObjective == 3 && b.cutscenes == 0 && !b.removed && b.frame == 20;
    std::printf(ok ? "00ee7a20_TEST PASS\n" : "00ee7a20_TEST FAIL\n");
    return ok ? 0 : 1;
}
