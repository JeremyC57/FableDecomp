// Behaviour test for CDF_GiveGift::Main (0x00e71980): demon-door face -- after taking scripted control, each frame: if the
// opening trigger holds (presented with a romantic gift), speak the opening line, open the door, reactivate the exit and finish; otherwise, if
// the hero talks to the face, speak the intro line and keep watching.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int talkFrame[2]; int triggerFrom;
    int intros; int openings; int doorsOpened; bool exitActive; int pauses; int unpauses; int speakFrames;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool Talked(const Sim& s) { return s.talkFrame[0] == s.frame || s.talkFrame[1] == s.frame; }
static bool Trigger(const Sim& s) { return s.frame >= s.triggerFrom; }
static void Speak(Sim& s, bool opening) { if (opening) ++s.openings; else ++s.intros; s.speakFrames = 2; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static bool Scene(Sim& s, bool opening)
{
    ++s.pauses;
    Speak(s, opening);
    while (Busy(s)) { Frame(s); if (Term(s)) { ++s.unpauses; return false; } }
    if (Term(s)) { ++s.unpauses; return false; }
    return true;
}
static void Main(Sim& s)
{
    Frame(s);
    if (Term(s)) return;
    while (s.frame < s.grantFrom) { Frame(s); if (Term(s)) return; }
    if (Term(s)) return;
    s.exitActive = false;
    while (!Term(s)) {
        if (Trigger(s)) {
            if (!Scene(s, true)) return;
            ++s.doorsOpened; s.exitActive = true; ++s.unpauses;
            return;
        } else if (Talked(s)) {
            if (!Scene(s, false)) return;
            ++s.unpauses;
        }
        Frame(s);
    }
}
int main()
{
    Sim a; std::memset(&a, 0, sizeof(a));
    a.terminateAt = 60; a.grantFrom = 3; a.talkFrame[0] = 5; a.talkFrame[1] = 12; a.triggerFrom = 1000;
    Sim b = a; b.triggerFrom = 20;
    Sim c = a; c.terminateAt = 6;            // terminates while the intro line plays
    Main(a); Main(b); Main(c);
    bool ok = a.intros == 2 && a.openings == 0 && !a.exitActive && a.pauses == a.unpauses && a.frame == 60
           && b.intros == 2 && b.openings == 1 && b.doorsOpened == 1 && b.exitActive && b.pauses == b.unpauses && b.frame < 60
           && c.intros == 1 && c.doorsOpened == 0 && c.pauses == 1 && c.unpauses == 1;
    std::printf(ok ? "00e71980_TEST PASS\n" : "00e71980_TEST FAIL\n");
    return ok ? 0 : 1;
}
