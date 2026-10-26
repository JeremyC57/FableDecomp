#include <cstdio>
#include <cstring>
#include <string>
#include <map>
// Behaviour test for CQ_WaspBossScript::WaspIntro (0x00E12F20): sets StartChase, plays CS_WASPBOSS_INTRO with HERO/VICTIM
// cast while non-scripted entities are paused, shows the quest start screen once, and always unpauses on exit.
class CScriptThing { public: int Id; };
struct CHandle { int Thing; CHandle() : Thing(0) {} };
struct CGSI {
    bool Paused; int PauseCalls; int Screens; char Log[128]; int Frames; CScriptThing Hero;
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int) { h.Thing = t.Id; return true; }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.Id = std::strcmp(n, "WaspVictim") ? 0 : 7; return t; }
    void StartMovieSequence(const char*, CHandle&) {}
    void PauseAllNonScriptedEntities(bool p) { Paused = p; ++PauseCalls; }
    std::string GetActiveQuestName() { return "Q_Wasp"; }
    void KickOffQuestStartScreen(const std::string& q, bool, bool) { if (q == "Q_Wasp") ++Screens; }
};
static CGSI* g_GSI;
static void RunCutsceneMacro(const char* name, std::map<std::string, CHandle>& cast, void*, void*, bool, bool)
{
    if (g_GSI->Paused && cast["HERO"].Thing == 1 && cast["VICTIM"].Thing == 7) std::strcpy(g_GSI->Log, name);
}
struct CQ_WaspBossScript {
    CGSI* GSI; bool StartChase; bool QuestStartScreened; bool Terminating;
    bool IsActiveThreadTerminating() { return Terminating; }
    void WaspIntro()
    {
        StartChase = true;
        CScriptThing victim = GSI->GetThingWithScriptName("WaspVictim");
        CHandle hero, victimHandle;
        GSI->StartScriptingEntity(GSI->GetHero(), hero, 4);
        GSI->StartScriptingEntity(victim, victimHandle, 4);
        std::map<std::string, CHandle> cast;
        cast["HERO"] = hero;
        cast["VICTIM"] = victimHandle;
        CHandle movie;
        GSI->StartMovieSequence("", movie);
        CGSI* game = GSI;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_WASPBOSS_INTRO", cast, 0, 0, false, true);
        if (!QuestStartScreened) {
            if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            GSI->KickOffQuestStartScreen(GSI->GetActiveQuestName(), true, false);
            QuestStartScreened = true;
        }
        game->PauseAllNonScriptedEntities(false);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.Hero.Id = 1; g_GSI = &gsi;
    CQ_WaspBossScript s; s.GSI = &gsi; s.StartChase = false; s.QuestStartScreened = false; s.Terminating = false;
    s.WaspIntro();
    s.WaspIntro();                        // second run: no second start screen
    s.QuestStartScreened = false; s.Terminating = true;
    s.WaspIntro();                        // terminating: no screen, still unpaused
    bool ok = s.StartChase && gsi.Screens == 1 && !gsi.Paused && gsi.PauseCalls == 6 && !std::strcmp(gsi.Log, "CS_WASPBOSS_INTRO")
              && !s.QuestStartScreened;
    if (!ok) { std::printf("00e12f20_TEST FAIL %d %d\n", gsi.Screens, gsi.PauseCalls); return 1; }
    std::printf("00e12f20_TEST PASS\n");
    return 0;
}