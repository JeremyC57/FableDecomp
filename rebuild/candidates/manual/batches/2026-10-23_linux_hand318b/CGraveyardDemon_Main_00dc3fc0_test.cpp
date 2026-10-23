// Behaviour test for CGraveyardDemon::Main (0x00dc3fc0): until the door changes, each hero talk plays a paused scene in which
// the demon speaks its first, second, then third line (the third repeats); the talk counter always advances.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int talkFrame[4]; int doorFrom; long timesSpoken; float health;
    int lines[3]; int pauses; int unpauses; int speakFrames;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool Talked(const Sim& s) { for (int i = 0; i < 4; ++i) if (s.talkFrame[i] == s.frame) return true; return false; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.grantFrom) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    while (s.frame < s.doorFrom) {
        Frame(s);
        if (Term(s)) return;
        if (Talked(s)) {
            if (Term(s)) return;
            ++s.pauses;
            long which = s.timesSpoken++;
            int line = which == 0 ? 0 : (which == 1 ? 1 : 2);
            if (s.health > 0.0f) {
                ++s.lines[line]; s.speakFrames = 2;
                while (Busy(s)) { Frame(s); if (Term(s)) { ++s.unpauses; return; } }
                if (Term(s)) { ++s.unpauses; return; }
            }
            ++s.unpauses;
        }
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.grantFrom = 3; a.talkFrame[0] = 5; a.talkFrame[1] = 12; a.talkFrame[2] = 20; a.talkFrame[3] = 30;
    a.doorFrom = 50; a.health = 1.0f;
    Sim b = a; b.health = 0.0f;                 // a dead demon stays silent but still counts the talks
    Sim c = a; c.terminateAt = 13;              // terminates while the second line plays
    Main(a); Main(b); Main(c);
    bool ok = a.lines[0] == 1 && a.lines[1] == 1 && a.lines[2] == 2 && a.timesSpoken == 4 && a.pauses == 4 && a.unpauses == 4 && a.frame == 50
           && b.lines[0] == 0 && b.lines[2] == 0 && b.timesSpoken == 4 && b.pauses == b.unpauses
           && c.lines[0] == 1 && c.lines[1] == 1 && c.timesSpoken == 2 && c.pauses == 2 && c.unpauses == 2;
    std::printf(ok ? "00dc3fc0_TEST PASS\n" : "00dc3fc0_TEST FAIL\n");
    return ok ? 0 : 1;
}
