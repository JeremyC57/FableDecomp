// Behaviour test for NScript::CQ_EndGameBossBattleScript::CEndGameSister::Main (0x00D328C0): the hero's sister in the final boss
// battle. She waits for the intro, is taken under script control and made untouchable and unconscious; once the final decision
// starts she is made targetable again (+10000 health), CS_JACK_DEATH plays and the hero gets the Sword of Aeons; then, until the
// decision is made, a sword hit from the hero plays CS_JACK_BOSS_ENDING_EVIL (JackBossBattleResult 1, FinalCutscenePlayed) and a
// cut-scene that needs her makes her give up and later retake her scripted control; afterwards she idles until the thread ends.
// A frame-stepped state model of the parent flags.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Sister {
    int Frame, TerminateAt, IntroAt, DecisionAt, SwordHitAt, NeededFrom, NeededTo;
    bool IntroFinished, FinalDecision, FinalDecisionMade, SisterNeeded, FinalCutscenePlayed, Targetable, Conscious, Scripted;
    int JackBossBattleResult, Health;
    void NSF()
    {
        ++Frame;
        if (Frame == IntroAt) IntroFinished = true;
        if (Frame == DecisionAt) FinalDecision = true;
        SisterNeeded = Frame >= NeededFrom && Frame < NeededTo;
    }
    bool T() const { return Frame >= TerminateAt; }
    void Main()
    {
        NSF(); if (T()) return;
        while (!IntroFinished) { NSF(); if (T()) return; }
        Scripted = true; Targetable = false; Conscious = false; Log("untouchable");
        while (!FinalDecision) { NSF(); if (T()) return; }
        Targetable = true; Health += 10000; Conscious = true;
        Log("fade"); Log("CS_JACK_DEATH"); Log("OBJECT_SWORD_OF_AEONS");
        while (!FinalDecisionMade) {
            NSF(); if (T()) return;
            if (Frame == SwordHitAt) {
                FinalDecisionMade = true; JackBossBattleResult = 1;
                Log("CS_JACK_BOSS_ENDING_EVIL"); FinalCutscenePlayed = true;
            }
            else if (SisterNeeded) {
                Scripted = false; Log("released");
                while (SisterNeeded) { NSF(); if (T()) return; }
                Scripted = true; Log("retaken");
            }
        }
        if (T()) return;
        for (;;) { NSF(); if (T()) return; }
    }
};
int main()
{
    Sister a = { 0, 40, 3, 6, 15, 9, 12 };
    a.Main();
    bool ok = !std::strcmp(g_log, "untouchable;fade;CS_JACK_DEATH;OBJECT_SWORD_OF_AEONS;released;retaken;CS_JACK_BOSS_ENDING_EVIL;")
        && a.JackBossBattleResult == 1 && a.FinalCutscenePlayed && a.Health == 10000 && a.Targetable && a.Conscious && a.Frame == 40;
    g_log[0] = 0;
    Sister b = { 0, 5, 30, 40, 50, 0, 0 };
    b.Main();
    ok = ok && g_log[0] == 0 && !b.Scripted && b.Frame == 5;
    std::printf(ok ? "00d328c0_TEST PASS\n" : "00d328c0_TEST FAIL\n");
    return ok ? 0 : 1;
}