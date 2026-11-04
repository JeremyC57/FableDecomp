// Behaviour test for CHKC_VillagerScreamer::Main (0x00e1b1f0): until the villagers turn on the hero, each talk plays the hint
// scene (when the villager has health); then, unless the maze is defeated, a screamer spawns, targets the hero and Me is removed.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int talkFrame[2]; int turnFrame; bool mazeDefeated; float health;
    int hints; int pauses; int unpauses; int speakFrames; int screamers; bool removed; int priority; bool info;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool Talked(const Sim& s) { return s.talkFrame[0] == s.frame || s.talkFrame[1] == s.frame; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    s.priority = 2;
    if (Term(s)) return;
    s.info = true;
    while (s.frame < s.turnFrame) {
        Frame(s);
        if (Term(s)) return;
        if (Talked(s)) {
            if (Term(s)) return;
            s.priority = 4;
            ++s.pauses;
            if (s.health > 0.0f) {
                ++s.hints; s.speakFrames = 2;
                while (Busy(s)) { Frame(s); if (Term(s)) { ++s.unpauses; return; } }
                if (Term(s)) { ++s.unpauses; return; }
            }
            ++s.unpauses;
            s.priority = 1;
        }
    }
    if (Term(s)) return;
    if (s.mazeDefeated) return;
    if (Term(s)) return;
    ++s.screamers;
    s.removed = true;
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.talkFrame[0] = 5; a.talkFrame[1] = 12; a.turnFrame = 30; a.health = 1.0f;
    Sim b = a; b.mazeDefeated = true;
    Sim c = a; c.terminateAt = 6;
    Main(a); Main(b); Main(c);
    bool ok = a.hints == 2 && a.pauses == 2 && a.unpauses == 2 && a.screamers == 1 && a.removed && a.priority == 1 && a.info
           && b.hints == 2 && b.screamers == 0 && !b.removed
           && c.hints == 1 && c.pauses == 1 && c.unpauses == 1 && c.screamers == 0 && c.priority == 4;
    std::printf(ok ? "00e1b1f0_TEST PASS\n" : "00e1b1f0_TEST FAIL\n");
    return ok ? 0 : 1;
}