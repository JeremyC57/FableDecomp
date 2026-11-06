// Behaviour test for NScript::CV_LostTraderScript::CLostTraderBrother::Main (0x00EA59E0): intro once (state 3, DoneIntro), the walk
// home with the follow-me stop when the hero lags (once per lag), state 4, the wait for the hero, the DEEPER line the first time
// (PLEASE afterwards only on a rand hit), talk answers by TalkCounter until the trader is reached, then the reunion: quest completed,
// morality given, both brothers removed. A simulation of the same control flow with a scripted game.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, stateBrain, talkCounter, distHome, heroLagUntil, heroNearAt, reachAt, talkAt[8], nTalk;
    bool doneIntro, told;
    bool T() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void NSF() { ++frame; }
    bool TalkedTo() { for (int i = 0; i < nTalk; ++i) if (talkAt[i] == frame) return true; return false; }
    void Main()
    {
        if (T()) return;
        if (!doneIntro) { Ev("INTRO"); stateBrain = 3; Ev("QUESTSTART"); doneIntro = true; }
        if (stateBrain == 3) {
            bool stopped = false;
            while (distHome > 2) {
                if (frame < heroLagUntil) { if (!stopped) { Ev("FOLLOWME"); stopped = true; } }
                else { --distHome; stopped = false; }
                NSF(); if (T()) return;
            }
            if (T()) return;
            stateBrain = 4;
        }
        if (stateBrain == 4) {
            while (frame < heroNearAt && frame < reachAt) { NSF(); if (T()) return; }
            if (T()) return;
            if (!told) { Ev("DEEPER"); told = true; }
            while (frame < reachAt) {
                if (TalkedTo()) {
                    switch (talkCounter) { case 0: Ev("T0"); break; case 1: Ev("T1"); break; case 2: Ev("T2"); break; case 3: Ev("T3"); break; default: Ev("TD"); break; }
                    ++talkCounter;
                }
                NSF(); if (T()) return;
            }
            if (T()) return;
            Ev("REUNION"); Ev("COMPLETED"); Ev("MORALITY"); Ev("REMOVE2");
        }
    }
};
static void Make(Sim& s) { std::memset(&s, 0, sizeof(s)); s.termFrame = 1000; s.distHome = 6; s.heroLagUntil = 2; s.heroNearAt = 10; s.reachAt = 20; s.nTalk = 2; s.talkAt[0] = 12; s.talkAt[1] = 15; }
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = std::strcmp(a.log, "INTRO;QUESTSTART;FOLLOWME;DEEPER;T0;T1;REUNION;COMPLETED;MORALITY;REMOVE2;") == 0 && a.stateBrain == 4 && a.talkCounter == 2;
    Sim b; Make(b); b.doneIntro = true; b.stateBrain = 4; b.told = true; b.Main();
    ok = ok && std::strcmp(b.log, "T0;T1;REUNION;COMPLETED;MORALITY;REMOVE2;") == 0;
    Sim c; Make(c); c.termFrame = 3; c.Main();
    ok = ok && std::strcmp(c.log, "INTRO;QUESTSTART;FOLLOWME;") == 0 && c.stateBrain == 3;
    std::printf(ok ? "00ea59e0_TEST PASS\n" : "00ea59e0_TEST FAIL\n");
    return ok ? 0 : 1;
}