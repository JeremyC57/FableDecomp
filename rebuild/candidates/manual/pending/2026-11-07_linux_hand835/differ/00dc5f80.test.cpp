// Behaviour test for NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main (0x00DC5F80): the first loop runs
// until the gravekeeper is introduced or she has been talked to (ASIDE line); a hit by the hero (or a special ability other than
// heal-life) sets AttackedByHero and VisitorsAttacked and plays ATTACKED; VisitorsAttacked alone plays RUN_AWAY; the second loop answers
// talk with LEAVING_FIRST once and LEAVING_SECOND afterwards, and fades her out whenever she is not performing a task.
#include <cstdio>
#include <cstring>
struct Sim {
    char log[1024]; int frame, termFrame, talkFrames[4], hitFrame, abilityFrame, healFrame, arriveFrame, introFrame;
    bool attackedByHero, visitorsAttacked; int talkSeen;   // a talk message is consumed by the frame it arrives in
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    bool Term() const { return frame >= termFrame; }
    bool Talk() const { for (int i = 0; i < 4; ++i) if (talkFrames[i] == frame) return true; return false; }
    bool Hit() const { return frame == hitFrame || (frame == abilityFrame && frame != healFrame); }
    void Main()
    {
        bool haveTalked = false;
        while (frame < introFrame && !haveTalked) {
            ++frame;
            if (Term()) return;
            if (Talk()) { Ev("ASIDE"); haveTalked = true; talkSeen = frame; }
            if (Hit()) { attackedByHero = true; visitorsAttacked = true; Ev("ATTACKED"); Ev("FADE"); }
            else if (visitorsAttacked) { attackedByHero = true; Ev("RUN_AWAY"); Ev("FADE"); }
        }
        if (Term()) return;
        Ev("WALK");
        bool haveSpoken = false;
        while (true) {
            if (frame > talkSeen && Talk()) { Ev(haveSpoken ? "LEAVING_SECOND" : "LEAVING_FIRST"); haveSpoken = true; Ev("WALK"); }
            if (frame >= arriveFrame) { Ev("FADE"); return; }   // the fade kills the entity, which ends the script
            ++frame;
            if (Term()) return;
        }
    }
};
static void Make(Sim& s)
{
    s.log[0] = 0; s.frame = 0; s.termFrame = 100; s.hitFrame = s.abilityFrame = s.healFrame = -1; s.arriveFrame = 50;
    s.introFrame = 1000; s.attackedByHero = s.visitorsAttacked = false; s.talkSeen = -1; for (int i = 0; i < 4; ++i) s.talkFrames[i] = -1;
}
int main()
{
    Sim a; Make(a); a.talkFrames[0] = 3; a.talkFrames[1] = 10; a.talkFrames[2] = 20; a.Main();
    bool ok = std::strcmp(a.log, "ASIDE;WALK;LEAVING_FIRST;WALK;LEAVING_SECOND;WALK;FADE;") == 0 && !a.attackedByHero;
    Sim b; Make(b); b.abilityFrame = 2; b.healFrame = 2; b.introFrame = 4; b.Main();       // heal-life is not an attack
    ok = ok && std::strcmp(b.log, "WALK;FADE;") == 0 && !b.visitorsAttacked;
    Sim c; Make(c); c.hitFrame = 2; c.talkFrames[0] = 2; c.Main();
    ok = ok && std::strcmp(c.log, "ASIDE;ATTACKED;FADE;WALK;FADE;") == 0 && c.attackedByHero && c.visitorsAttacked;
    Sim d; Make(d); d.visitorsAttacked = true; d.introFrame = 1; d.Main();
    ok = ok && std::strcmp(d.log, "RUN_AWAY;FADE;WALK;FADE;") == 0 && d.attackedByHero;
    Sim e; Make(e); e.termFrame = 2; e.Main();
    ok = ok && e.log[0] == 0;
    std::printf(ok ? "00dc5f80_TEST PASS\n" : "00dc5f80_TEST FAIL\n");
    return ok ? 0 : 1;
}
