// Behaviour test for NScript::CV_DemonDoorsScript::CDF_Dragon::Main (0x00E6FC40): demon-door dragon face. After taking scripted
// control it shuts the region exit; each frame a hero talk opens a paused cutscene whose line depends on the renown level
// (0,1 ZERO; 2,3 ONE; 4,5 TWO; 6 THREE; 7 FOUR; others: no line), spoken only while the face is alive; renown 7 then opens the
// door, reopens the exit and activates Q_DragonBossFight and the script ends. Every pause is undone, also on a terminate.
#include <cstdio>
#include <cstring>

struct Sim {
    int frame; int terminateAt; int grantFrom; int talkFrame[2]; long renown; float health;
    int lines[5]; int doorsOpened; bool exitActive; bool questActive; int pauses; int unpauses; int speakFrames; int movies;
};
static bool Term(const Sim& s) { return s.frame >= s.terminateAt; }
static void Frame(Sim& s) { ++s.frame; }
static bool Talked(const Sim& s) { return s.talkFrame[0] == s.frame || s.talkFrame[1] == s.frame; }
static bool Busy(Sim& s) { if (s.speakFrames > 0) { --s.speakFrames; return true; } return false; }
static void Speak(Sim& s, int line) { ++s.lines[line]; s.speakFrames = 2; }

struct Pause {                                  // CWideScreenMagicPauseEntities
    Sim& s;
    Pause(Sim& sim) : s(sim) { ++s.pauses; }
    ~Pause() { ++s.unpauses; }
};
// returns false on a terminate inside the line (the caller returns)
static bool Line(Sim& s, int line)
{
    if (s.health > 0.0f) {
        Speak(s, line);
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
            if (Term(s)) return;
            ++s.movies;
            Pause pause(s);
            switch (s.renown) {
            case 0: case 1: if (!Line(s, 0)) return; break;
            case 2: case 3: if (!Line(s, 1)) return; break;
            case 4: case 5: if (!Line(s, 2)) return; break;
            case 6: if (!Line(s, 3)) return; break;
            case 7:
                if (!Line(s, 4)) return;
                ++s.doorsOpened; s.exitActive = true; s.questActive = true;
                return;
            }
        }
        Frame(s);
    }
}
int main()
{
    Sim base; std::memset(&base, 0, sizeof(base));
    base.terminateAt = 40; base.grantFrom = 3; base.talkFrame[0] = 5; base.talkFrame[1] = 12; base.health = 10.0f;
    int expect[8] = { 0, 0, 1, 1, 2, 2, 3, 4 };
    for (int r = 0; r < 8; ++r) {
        Sim s = base; s.renown = r;
        Main(s);
        int talks = (r == 7) ? 1 : 2;
        if (s.lines[expect[r]] != talks || s.pauses != s.unpauses || s.pauses != talks
            || s.exitActive != (r == 7) || s.questActive != (r == 7) || s.doorsOpened != (r == 7 ? 1 : 0)) {
            std::printf("00e6fc40_TEST FAIL renown %d\n", r); return 1;
        }
    }
    Sim d = base; d.renown = 9; Main(d);             // out of the table: no line, still paused and unpaused
    Sim h = base; h.renown = 7; h.health = 0.0f; Main(h);   // dead face: no line, the door still opens
    Sim t = base; t.renown = 3; t.terminateAt = 6; Main(t); // terminate while the line plays
    bool ok = d.lines[0] + d.lines[1] + d.lines[2] + d.lines[3] + d.lines[4] == 0 && d.pauses == 2 && d.unpauses == 2 && d.frame == 40
           && h.lines[4] == 0 && h.doorsOpened == 1 && h.questActive && h.pauses == 1 && h.unpauses == 1
           && t.lines[1] == 1 && t.pauses == 1 && t.unpauses == 1 && t.doorsOpened == 0 && !t.exitActive;
    std::printf(ok ? "00e6fc40_TEST PASS\n" : "00e6fc40_TEST FAIL\n");
    return ok ? 0 : 1;
}