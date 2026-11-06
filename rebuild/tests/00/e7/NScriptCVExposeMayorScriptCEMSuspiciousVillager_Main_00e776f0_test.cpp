// Behaviour test for NScript::CV_ExposeMayorScript::CEM_SuspiciousVillager::Main (0x00E776F0): the prisoner's frame loop until
// stage 5 -- the 7-second attention line when the hero is near (ATTENTION before the intro, TRAPPED after), stage 4 outro (path 1:
// wait to be talked to, cut-scene once then the OUTRO line) or removal (path 2), the attacked cut-scene, and the talk-to dispatch by
// stage (stage 0: married intro, or intro / ask-again plus the yes-no question; yes sets stage 1).
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, timer, stage, path, answer, talkFrame, hitFrame, nearFrame;
    bool doneIntro, doneOutro, married, talkedAboutLamp;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Talked() const { return frame == talkFrame; }
    void Play(const char* cs) { Ev(cs); }
    void Main()
    {
        if (Term()) return;
        timer = 0;
        while (stage != 5) {
            ++frame; if (timer > 0) --timer;
            if (Term()) return;
            if (timer == 0 && frame >= nearFrame) {
                if (Term()) return;
                timer = 7;
                Ev(doneIntro ? "TRAPPED" : "ATTENTION");
            }
            if (stage == 4) {
                if (Term()) return;
                if (path == 1) {
                    if (Term()) return;
                    while (!Talked()) { ++frame; if (Term()) return; }
                    if (Term()) return;
                    if (doneOutro) Ev("OUTRO"); else { Play("CS_OUTRO"); doneOutro = true; }
                } else if (path == 2) { if (Term()) return; Ev("REMOVE"); stage = 5; continue; }
            }
            if (frame == hitFrame) { if (Term()) return; Play("CS_ATTACKED"); }
            if (Talked()) {
                if (Term()) return;
                if (stage == 0) {
                    if (married) Play("CS_INTRO_MARRIED");
                    else {
                        if (doneIntro) Play("CS_ASKAGAIN"); else { Play("CS_INTRO"); doneIntro = true; }
                        Ev("QUESTION");
                        if (answer == 1) { Play("CS_ACCEPT"); stage = 1; } else Play("CS_REFUSE");
                    }
                } else if (married) Play("CS_MARRIED");
                else if (stage == 1) Play("CS_RHODRI");
                else if (stage == 2) { if (!talkedAboutLamp) { Play("CS_FLASHING"); talkedAboutLamp = true; } else Play("CS_FLASHING_SHORT"); }
                else if (stage == 3) Play("CS_GHOST");
            }
            if (frame >= 40) stage = 5;
        }
    }
};
static void Make(Sim& s)
{
    s.log[0] = 0; s.frame = 0; s.termFrame = 1000; s.timer = 0; s.stage = 0; s.path = 0; s.answer = 1; s.talkFrame = 3; s.hitFrame = -1;
    s.nearFrame = 100; s.doneIntro = s.doneOutro = s.married = s.talkedAboutLamp = false;
}
int main()
{
    Sim a; Make(a); a.nearFrame = 1; a.hitFrame = 2; a.Main();
    bool ok = std::strcmp(a.log, "ATTENTION;CS_ATTACKED;CS_INTRO;QUESTION;CS_ACCEPT;TRAPPED;TRAPPED;TRAPPED;TRAPPED;TRAPPED;") == 0 && a.stage == 5;
    Sim b; Make(b); b.married = true; b.Main();
    ok = ok && std::strcmp(b.log, "CS_INTRO_MARRIED;") == 0;
    Sim c; Make(c); c.stage = 4; c.path = 1; c.talkFrame = 5; c.termFrame = 6; c.Main();
    ok = ok && std::strcmp(c.log, "CS_OUTRO;") == 0 && c.doneOutro;
    Sim d; Make(d); d.stage = 2; d.Main();
    ok = ok && std::strcmp(d.log, "CS_FLASHING;") == 0 && d.talkedAboutLamp;
    Sim e; Make(e); e.answer = 0; e.doneIntro = true; e.Main();
    ok = ok && std::strcmp(e.log, "CS_ASKAGAIN;QUESTION;CS_REFUSE;") == 0 && e.stage == 5;
    std::printf(ok ? "00e776f0_TEST PASS\n" : "00e776f0_TEST FAIL\n");
    return ok ? 0 : 1;
}