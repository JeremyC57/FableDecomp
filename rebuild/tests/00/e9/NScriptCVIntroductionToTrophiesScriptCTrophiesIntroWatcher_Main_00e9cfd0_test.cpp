// Behaviour test for NScript::CV_IntroductionToTrophiesScript::CTrophiesIntroWatcher::Main (0x00E9CFD0): the crowd watcher
// keeps running while Briar Rose lives and is near; during the cutscene it gives up scripted control and retakes it after; a
// hero talk (unless the hero attacked) plays one paused crowd line; inside the tavern, three seconds off camera turn it towards
// Briar Rose and restart the timer, coming back on camera stops the timer.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int briarDiesAt; int talkFrame; bool attacked; int cutsceneFrom, cutsceneTo;
    int insideFrom; int offCameraFrom, offCameraTo; float health;
    int controlTaken; int controlReleased; int lines; int pauses; int unpauses; int facings; int speakFrames;
    bool timerStarted; int timerLeft;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.timerLeft > 0) --s.timerLeft; }
static bool Cutscene(const Sim& s) { return s.frame >= s.cutsceneFrom && s.frame < s.cutsceneTo; }
static bool OnScreen(const Sim& s) { return !(s.frame >= s.offCameraFrom && s.frame < s.offCameraTo); }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
struct Pause { Sim& s; Pause(Sim& sim) : s(sim) { ++s.pauses; } ~Pause() { ++s.unpauses; } };
static void Main(Sim& s)
{
    ++s.controlTaken;
    while (s.frame < s.briarDiesAt) {
        Frame(s);
        if (Term(s)) return;
        if (Cutscene(s)) {
            ++s.controlReleased;
            while (Cutscene(s)) { Frame(s); if (Term(s)) return; }
            ++s.controlTaken;
        }
        if (s.frame == s.talkFrame && !s.attacked) {
            Pause pause(s);
            if (s.health > 0.0f) {
                ++s.lines; s.speakFrames = 2;
                while (Busy(s)) { Frame(s); if (Term(s)) return; }
                if (Term(s)) return;
            }
        } else if (s.frame >= s.insideFrom) {
            if (!OnScreen(s) && !s.timerStarted) { s.timerStarted = true; s.timerLeft = 3; }
            else if (s.timerStarted && OnScreen(s)) { s.timerStarted = false; s.timerLeft = 0; }
            if (s.timerStarted && s.timerLeft == 0) {
                Frame(s); Frame(s); ++s.facings; Frame(s); Frame(s);
                s.timerStarted = false; s.timerLeft = 3;
            }
        }
    }
}
int main()
{
    Sim base; std::memset(&base, 0, sizeof(base));
    base.terminateAt = 100; base.briarDiesAt = 60; base.talkFrame = 5; base.cutsceneFrom = 10; base.cutsceneTo = 14;
    base.insideFrom = 20; base.offCameraFrom = 30; base.offCameraTo = 45; base.health = 1.0f; base.offCameraTo = 37;
    Sim a = base; Main(a);
    Sim b = base; b.attacked = true; Main(b);             // no crowd line after an attack
    Sim c = base; c.terminateAt = 6; Main(c);             // terminated inside the line: the pause is undone
    Sim d = base; d.offCameraTo = 31; Main(d);            // back on camera before the timer runs out: no turn
    bool ok = a.lines == 1 && a.pauses == 1 && a.unpauses == 1 && a.controlReleased == 1 && a.controlTaken == 2 && a.facings == 1
           && b.lines == 0 && b.pauses == 0 && b.facings == 1
           && c.lines == 1 && c.pauses == 1 && c.unpauses == 1 && c.facings == 0
           && d.facings == 0 && !d.timerStarted;
    std::printf(ok ? "00e9cfd0_TEST PASS\n" : "00e9cfd0_TEST FAIL\n");
    return ok ? 0 : 1;
}