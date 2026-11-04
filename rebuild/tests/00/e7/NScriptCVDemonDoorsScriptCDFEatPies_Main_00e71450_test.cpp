// Behaviour test for CDF_EatPies::Main (0x00e71450): demon-door face -- take scripted control (retried per frame), set Me
// persistent and immune, shut the nearest exit, then on each "talked to by the hero" either speak the intro line (test
// passes: keep looping) or speak the opening line, open the door, reactivate the exit and finish.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int talkFrame[4]; int talks;
    float value; float level;
    int intros; int openings; int doorsOpened; bool exitActive; int pauses; int unpauses; bool persistent; int speakFrames;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool Talked(Sim& s) { for (int i = 0; i < 4; ++i) if (s.talkFrame[i] == s.frame) return true; return false; }
static bool Test(const Sim& s) { return !(s.value >= s.level); }
static void Speak(Sim& s, bool opening) { if (opening) ++s.openings; else ++s.intros; s.speakFrames = 2; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.grantFrom) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.persistent = true; s.exitActive = false;
    while (!Term(s)) {
        if (Talked(s)) {
            if (Term(s)) break;
            ++s.pauses;
            if (Test(s)) {
                if (Term(s)) { ++s.unpauses; return; }
                Speak(s, false);
                while (Busy(s)) { Frame(s); if (Term(s)) { ++s.unpauses; return; } }
                if (Term(s)) { ++s.unpauses; return; }
                ++s.unpauses;
            } else {
                if (Term(s)) { ++s.unpauses; return; }
                Speak(s, true);
                while (Busy(s)) { Frame(s); if (Term(s)) { ++s.unpauses; return; } }
                if (Term(s)) { ++s.unpauses; return; }
                ++s.doorsOpened; s.exitActive = true; ++s.unpauses;
                break;
            }
        }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 100; a.grantFrom = 3; a.talkFrame[0] = 5; a.talkFrame[1] = 20; a.talkFrame[2] = -1; a.talkFrame[3] = -1;
    a.value = 0.2f; a.level = 0.5f;
    // first talk: intro (test holds); then the hero meets the condition
    Sim b = a;
    Main(a);
    bool ok = a.intros == 2 && a.openings == 0 && a.doorsOpened == 0 && !a.exitActive && a.pauses == a.unpauses && a.frame == 100;
    b.value = 0.9f;
    Main(b);
    ok = ok && b.intros == 0 && b.openings == 1 && b.doorsOpened == 1 && b.exitActive && b.pauses == 1 && b.unpauses == 1 && b.frame < 100;
    std::printf(ok ? "00e71450_TEST PASS\n" : "00e71450_TEST FAIL\n");
    return ok ? 0 : 1;
}