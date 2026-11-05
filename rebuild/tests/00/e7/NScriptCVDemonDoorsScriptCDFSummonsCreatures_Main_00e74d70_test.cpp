// Behaviour test for NScript::CV_DemonDoorsScript::CDF_SummonsCreatures::Main (0x00E74D70): demon-door summoning face. A hero
// talk plays a paused warning, asks a yes/no question and waits for the answer; on yes the intro plays, then for each creature
// type one creature spawns at every generator marker and the face waits until all spawned creatures are dead; a living face then
// plays the paused opening, opens its door and reopens the exit; on no the face keeps watching. Every pause is undone.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int talkFrame[2]; long answers[2]; int answerDelay; int answerCalls;
    int talks; long numTypes; int numGenerators; int spawned[4]; int killFrames; bool faceAlive; float health;
    int warnings; int questions; int intros; int openings; int doorsOpened; bool exitActive; int pauses; int unpauses;
    int speakFrames; int livingCreatures;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; if (s.livingCreatures > 0 && --s.killFrames <= 0) { s.livingCreatures = 0; } }
static bool Talked(const Sim& s) { return s.talkFrame[0] == s.frame || s.talkFrame[1] == s.frame; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static long Answer(Sim& s) { if (++s.answerCalls <= s.answerDelay) return -1; return s.answers[s.talks - 1]; }
struct Pause { Sim& s; Pause(Sim& sim) : s(sim) { ++s.pauses; } ~Pause() { ++s.unpauses; } };
static bool Line(Sim& s, int& counter)
{
    if (s.health > 0.0f) {
        ++counter; s.speakFrames = 2;
        while (Busy(s)) { Frame(s); if (Term(s)) return false; }
        if (Term(s)) return false;
    }
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
        if (Talked(s)) {
            ++s.talks; s.answerCalls = 0;
            if (Term(s)) return;
            long answer;
            {
                Pause pause(s);
                if (!Line(s, s.warnings)) return;
                ++s.questions;
                answer = Answer(s);
                while (answer < 0) { Frame(s); if (Term(s)) return; answer = Answer(s); }
                if (Term(s)) return;
                if (answer == 1) {
                    if (Term(s)) return;
                    if (!Line(s, s.intros)) return;
                }
            }
            if (answer == 1) {
                if (Term(s)) return;
                for (long i = 0; i < s.numTypes; ++i) {
                    if (Term(s)) return;
                    for (int j = 0; j < s.numGenerators; ++j) { ++s.spawned[i]; ++s.livingCreatures; }
                    s.killFrames = 3;
                    while (s.livingCreatures != 0) { Frame(s); if (Term(s)) return; }
                    if (Term(s)) return;
                }
                if (Term(s)) return;
                if (!s.faceAlive) {
                    if (Term(s)) return;
                } else {
                    Pause pause(s);
                    if (!Line(s, s.openings)) return;
                    ++s.doorsOpened; s.exitActive = true;
                }
                return;
            }
        }
        Frame(s);
    }
}
int main()
{
    Sim base; std::memset(&base, 0, sizeof(base));
    base.terminateAt = 80; base.grantFrom = 3; base.talkFrame[0] = 5; base.talkFrame[1] = 20; base.answers[0] = 0;
    base.answers[1] = 1; base.answerDelay = 2; base.numTypes = 3; base.numGenerators = 2; base.faceAlive = true; base.health = 5.0f;
    Sim a = base; Main(a);                               // no, then yes: three waves of two, then the door
    Sim b = base; b.faceAlive = false; Main(b);          // yes, but the face died: no opening, no door
    Sim c = base; c.answers[1] = 0; Main(c);             // no twice: keeps watching until terminated
    Sim d = base; d.terminateAt = 9; Main(d);            // terminated while waiting for the answer
    bool ok = a.talks == 2 && a.warnings == 2 && a.questions == 2 && a.intros == 1 && a.openings == 1 && a.doorsOpened == 1
           && a.exitActive && a.spawned[0] == 2 && a.spawned[1] == 2 && a.spawned[2] == 2 && a.pauses == 3 && a.unpauses == 3
           && b.doorsOpened == 0 && b.openings == 0 && !b.exitActive && b.spawned[2] == 2 && b.pauses == 2 && b.unpauses == 2
           && c.talks == 2 && c.intros == 0 && c.spawned[0] == 0 && c.frame == 80 && c.pauses == 2 && c.unpauses == 2
           && d.talks == 1 && d.questions == 1 && d.pauses == 1 && d.unpauses == 1 && d.spawned[0] == 0;
    std::printf(ok ? "00e74d70_TEST PASS\n" : "00e74d70_TEST FAIL\n");
    return ok ? 0 : 1;
}