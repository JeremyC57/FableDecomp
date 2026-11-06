// Behaviour test for NScript::CQ_DragonBossFightScript::DoMission (0x00D26190): the region wait, the followers put in limbo and
// taken out again, the DragonState == 4 wait, the PutMaskOn branch (evil: HeroWoreMask, morality -1; good: morality +0.5), the
// four frames before the movie, the movie chosen by GetHeroMorality() < 0, and the terminate exits (no further events).
#include <cstdio>
#include <cstring>
struct Sim {
    char log[2048]; int frame, termFrame, regionFrame, deadFrame, followers, inLimbo; bool putMaskOn, heroWoreMask, bodyGuardsInLimbo;
    float morality; int dragonState;
    bool Term() const { return frame >= termFrame; }
    void Ev(const char* e) { std::strcat(log, e); std::strcat(log, ";"); }
    void NewScriptFrame() { ++frame; if (frame >= deadFrame) dragonState = 4; }
    void DoMission()
    {
        Ev("OBJ01");
        while (frame < regionFrame) { NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        Ev("FADEOUT"); bodyGuardsInLimbo = true;
        for (int i = 0; i < followers; ++i) ++inLimbo;
        Ev("CS_DRAGON_INTRO"); Ev("OBJ02"); Ev("HEALTHBAR"); Ev("THREADS");
        while (dragonState != 4) { NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        Ev("CS_DRAGON_DEATH"); Ev("CS_DRAGON_OUTRO_CHOICE");
        bool mask = putMaskOn;
        if (Term()) return;
        Ev(mask ? "CS_DRAGON_OUTRO_EVIL" : "CS_DRAGON_OUTRO_GOOD");
        heroWoreMask = mask;
        for (int i = 0; i < followers; ++i) --inLimbo;
        Ev("COMPLETED");
        for (int k = 0; k < 4; ++k) { NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        if (morality < 0.5f) Ev(mask ? "evil_mask" : "evil_no_mask"); else Ev(mask ? "good_mask" : "good_no_mask");
        morality += mask ? -1.0f : 0.5f;
        Ev("DEACTIVATE");
    }
};
static void Make(Sim& s) { s.log[0] = 0; s.frame = 0; s.termFrame = 1000; s.regionFrame = 2; s.deadFrame = 5; s.followers = 3; s.inLimbo = 0;
    s.putMaskOn = false; s.heroWoreMask = true; s.bodyGuardsInLimbo = false; s.morality = 0.75f; s.dragonState = 0; }
int main()
{
    Sim a; Make(a); a.putMaskOn = true; a.morality = -0.5f; a.DoMission();
    bool ok = std::strcmp(a.log, "OBJ01;FADEOUT;CS_DRAGON_INTRO;OBJ02;HEALTHBAR;THREADS;CS_DRAGON_DEATH;CS_DRAGON_OUTRO_CHOICE;CS_DRAGON_OUTRO_EVIL;COMPLETED;evil_mask;DEACTIVATE;") == 0
        && a.heroWoreMask && a.inLimbo == 0 && a.bodyGuardsInLimbo && a.morality < -1.4f && a.frame == 9;
    Sim b; Make(b); b.DoMission();
    ok = ok && std::strcmp(b.log, "OBJ01;FADEOUT;CS_DRAGON_INTRO;OBJ02;HEALTHBAR;THREADS;CS_DRAGON_DEATH;CS_DRAGON_OUTRO_CHOICE;CS_DRAGON_OUTRO_GOOD;COMPLETED;good_no_mask;DEACTIVATE;") == 0
        && !b.heroWoreMask && b.morality > 0.7f;
    Sim c; Make(c); c.termFrame = 1; c.DoMission();
    ok = ok && std::strcmp(c.log, "OBJ01;") == 0 && !c.bodyGuardsInLimbo;
    Sim d; Make(d); d.termFrame = 4; d.DoMission();
    ok = ok && std::strcmp(d.log, "OBJ01;FADEOUT;CS_DRAGON_INTRO;OBJ02;HEALTHBAR;THREADS;") == 0 && d.inLimbo == 3;
    std::printf(ok ? "00d26190_TEST PASS\n" : "00d26190_TEST FAIL\n");
    return ok ? 0 : 1;
}