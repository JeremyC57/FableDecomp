// Behaviour test for NScript::CV_MayorsInvitationScript::CMI_InfoKeeper::Main (0x00EB8900): nothing once the secret is learned, idle
// after speaking, the gossip intro whenever talked to before WhichQuest 3, then the chain number from the data string, information
// offered, and on the next talk the gossip line chosen by SecretsLearned as a float (0..3), counted; the fourth sets HeroLearnedSecret
// and objective 04; information cleared and HaveSpoken set.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct Sim {
    char log[1024]; int frame, termFrame, whichQuestFrame, chain; int WhichQuest; long SecretsLearned;
    bool HeroLearnedSecret, HaveSpoken; const char* data; int talkA, talkB;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void Main()
    {
        if (Term()) return;
        if (HeroLearnedSecret) return;
        if (HaveSpoken) { Ev("CLEAR"); for (;;) { ++frame; if (Term()) return; } }
        while (WhichQuest != 3) {
            ++frame; if (frame >= whichQuestFrame) WhichQuest = 3;
            if (Term()) return;
            if (frame == talkA) Ev("INTRO");
        }
        if (Term()) return;
        chain = std::atoi(data); Ev("INFO");
        while (frame < talkB) { ++frame; if (Term()) return; }
        float secrets = (float)SecretsLearned;
        if (secrets == 0.0f) { Ev("G1"); ++SecretsLearned; Ev("CLEAR"); }
        else if (secrets == 1.0f) { Ev("G2"); ++SecretsLearned; Ev("CLEAR"); }
        else if (secrets == 2.0f) { Ev("G3"); ++SecretsLearned; Ev("CLEAR"); }
        else if (secrets == 3.0f) { Ev("G4"); HeroLearnedSecret = true; Ev("OBJ04"); ++SecretsLearned; Ev("CLEAR"); }
        Ev("CLEAR"); HaveSpoken = true;
        for (;;) { ++frame; if (Term()) return; }
    }
};
static void Make(Sim& s)
{
    s.log[0] = 0; s.frame = 0; s.termFrame = 20; s.whichQuestFrame = 4; s.chain = 0; s.WhichQuest = 0; s.SecretsLearned = 0;
    s.HeroLearnedSecret = s.HaveSpoken = false; s.data = "2"; s.talkA = 2; s.talkB = 6;
}
int main()
{
    Sim a; Make(a); a.Main();
    bool ok = std::strcmp(a.log, "INTRO;INFO;G1;CLEAR;CLEAR;") == 0 && a.SecretsLearned == 1 && a.HaveSpoken && a.chain == 2;
    Sim b; Make(b); b.SecretsLearned = 3; b.Main();
    ok = ok && std::strcmp(b.log, "INTRO;INFO;G4;OBJ04;CLEAR;CLEAR;") == 0 && b.HeroLearnedSecret && b.SecretsLearned == 4;
    Sim c; Make(c); c.HeroLearnedSecret = true; c.Main();
    ok = ok && c.log[0] == 0 && c.frame == 0;
    Sim d; Make(d); d.HaveSpoken = true; d.Main();
    ok = ok && std::strcmp(d.log, "CLEAR;") == 0 && d.frame == 20;
    Sim e; Make(e); e.SecretsLearned = 7; e.Main();
    ok = ok && std::strcmp(e.log, "INTRO;INFO;CLEAR;") == 0 && e.SecretsLearned == 7;
    std::printf(ok ? "00eb8900_TEST PASS\n" : "00eb8900_TEST FAIL\n");
    return ok ? 0 : 1;
}