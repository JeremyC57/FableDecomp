// Behaviour test for NScript::CV_KnotholeGladeGatesScript::DoInitialBalverineAttack (0x00EA1720): spawn the surprise balverine
// at the first balverine point and sound the gate horn, wait (one script frame at a time) until FirstBalvDead; then, if a
// villager is still outside, make the hero the opinion source and target, wait until the point is off screen and the hero is in
// control, spawn the persistent murderer balverine near the point with the horn, give it the hero as enemy, make it an opinion
// source, flag IntroductionFinished, wait until it is dead and unset the hero as opinion source; with no villager outside the
// same without the opinion steps. Every terminate check stops the function. A log model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script;
struct CGame {
    Script* Owner;
    int Frames, FirstDeadAt, OffScreenAt, ControlAt, MurdererDeadAt, TerminateAt;
    bool VillagerAlive;
    void NewScriptFrame();
    void CreateCreature(const char* def, const char* name) { Log("create"); Log(def); Log(name); }
    void CreateCreatureNearby(const char* def, float r, const char* name) { Log("nearby"); Log(def); Log(name); if (r != 0.5f) Log("BADR"); }
    void PlayCriteriaSoundOnThing(const char* s) { Log("sound"); Log(s); }
    void EntitySetAsOpinionSource(const char* who, const char* src) { Log("opinion"); Log(who); Log(src); }
    void EntityUnsetAsOpinionSource(const char* who) { Log("unset"); Log(who); }
    void SetThingPersistent(const char* who) { Log("persist"); Log(who); }
    void GiveThingBestEnemyTarget(const char* who, const char* t) { Log("enemy"); Log(who); Log(t); }
    bool IsCameraPosOnScreen() { return Frames < OffScreenAt; }
    bool IsHeroControlledByPlayer() { return Frames >= ControlAt; }
    bool MurdererAlive() { return Frames < MurdererDeadAt; }
};
struct Script {
    CGame* Game;
    bool FirstBalvDead, IntroductionFinished, Terminating;
    bool IsActiveThreadTerminating() { return Terminating; }
    void DoInitialBalverineAttack()
    {
        Game->CreateCreature("CREATURE_BALVERINE_01", "KGG_SurpriseBalverine");
        Game->PlayCriteriaSoundOnThing("KNG_HORN");
        FirstBalvDead = false;
        while (!FirstBalvDead) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        const bool villager = Game->VillagerAlive;
        if (IsActiveThreadTerminating()) return;
        if (villager) Game->EntitySetAsOpinionSource("hero", "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
        while (Game->IsCameraPosOnScreen() || !Game->IsHeroControlledByPlayer()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->CreateCreatureNearby("CREATURE_BALVERINE_01", 0.5f, "KGG_MurdererBalverine");
        Game->PlayCriteriaSoundOnThing("KNG_HORN");
        Game->SetThingPersistent("balv");
        Game->GiveThingBestEnemyTarget("balv", "hero");
        if (villager) Game->EntitySetAsOpinionSource("balv", "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
        IntroductionFinished = true;
        while (Game->MurdererAlive()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        if (villager) Game->EntityUnsetAsOpinionSource("hero");
    }
};
void CGame::NewScriptFrame()
{
    ++Frames; Log("frame");
    if (Frames == FirstDeadAt) Owner->FirstBalvDead = true;
    if (Frames == TerminateAt) Owner->Terminating = true;
}
static bool Run(bool villager, int firstDead, int offScreen, int control, int murdererDead, int term, bool intro, const char* expected)
{
    CGame g; Script s;
    g.Owner = &s; g.Frames = 0; g.FirstDeadAt = firstDead; g.OffScreenAt = offScreen; g.ControlAt = control;
    g.MurdererDeadAt = murdererDead; g.TerminateAt = term; g.VillagerAlive = villager;
    s.Game = &g; s.FirstBalvDead = true; s.IntroductionFinished = false; s.Terminating = false;
    g_log[0] = 0;
    s.DoInitialBalverineAttack();
    if (std::strcmp(g_log, expected) != 0 || s.IntroductionFinished != intro) { std::printf("%s\n", g_log); return false; }
    return true;
}
#define PRE "create;CREATURE_BALVERINE_01;KGG_SurpriseBalverine;sound;KNG_HORN;"
#define SPAWN "nearby;CREATURE_BALVERINE_01;KGG_MurdererBalverine;sound;KNG_HORN;persist;balv;enemy;balv;hero;"
int main()
{
    bool ok =
        // villager outside: opinion source, camera wait two frames, murderer dies after four frames, unset
        Run(true, 1, 3, 0, 5, 0, true, PRE "frame;opinion;hero;OPINION_SOURCE_INANIMATE_EVIL_HIGH;frame;frame;" SPAWN
                                       "opinion;balv;OPINION_SOURCE_INANIMATE_EVIL_HIGH;frame;frame;unset;hero;")
        // no villager: no opinion steps, waits until the hero is in control
        && Run(false, 2, 0, 3, 4, 0, true, PRE "frame;frame;frame;" SPAWN "frame;")
        // terminated while waiting for the first balverine
        && Run(true, 9, 0, 0, 0, 2, false, PRE "frame;frame;")
        // terminated while the murderer balverine is alive: no unset
        && Run(true, 1, 0, 0, 9, 3, true, PRE "frame;opinion;hero;OPINION_SOURCE_INANIMATE_EVIL_HIGH;" SPAWN
                                           "opinion;balv;OPINION_SOURCE_INANIMATE_EVIL_HIGH;frame;frame;");
    if (!ok) { std::printf("00ea1720_TEST FAIL\n"); return 1; }
    std::printf("00ea1720_TEST PASS\n");
    return 0;
}
