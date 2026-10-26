// Behaviour test for CV_MayorsInvitationScript::MonitorThunderHeroDuel (0x00EB70F0).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CMasterScriptData { bool HeroWonThunderDuel, HeroLostThunderDuel, ThunderDuelResolved; int outcome; };
class CGameScriptInterface { public:
    CMasterScriptData* data;
    bool active, completed;
    int frame;
    void NewScriptFrame()
    {
        ++frame;
        if (frame == 2) {
            if (data->outcome == 0) data->HeroWonThunderDuel = true;
            if (data->outcome == 1) data->HeroLostThunderDuel = true;
            if (data->outcome == 2) data->ThunderDuelResolved = true;
        }
    }
    void ActivateQuest(const char* q) { Log("activate"); Log(q); active = true; }
    void DeactivateQuest(const char* q, unsigned long) { Log("deactivate"); Log(q); }
    bool IsQuestActive(const char*) { return active; }
    bool IsQuestCompleted(const char*) { return completed; }
    void SetQuestCardObjective(const char*, const char* o, const char* r, const char*) { Log(o); Log(r); }
    const char* GetActiveQuestName() { return "V_MayorsInvitation"; }
};
struct CV_MayorsInvitationScript {
    CGameScriptInterface* Game;
    CMasterScriptData* PMasterData;
    bool HeroHasDefeatedThunder, HeroHasLostToThunder;
    long MonitorThunderHeroDuelRunning;
    bool IsActiveThreadTerminating() { return false; }
    void MonitorThunderHeroDuel()
    {
        if (MonitorThunderHeroDuelRunning) { if (IsActiveThreadTerminating()) return; return; }
        if (IsActiveThreadTerminating()) return;
        MonitorThunderHeroDuelRunning = 1;
        if (!PMasterData->ThunderDuelResolved) {
            if (IsActiveThreadTerminating()) return;
            Game->SetQuestCardObjective(Game->GetActiveQuestName(), "OBJECTIVE_07", "BowerstoneJail", "");
        }
        if (!Game->IsQuestActive("V_HeroDuel") && !Game->IsQuestCompleted("V_HeroDuel"))
            Game->ActivateQuest("V_HeroDuel");
        while (!PMasterData->HeroWonThunderDuel && !PMasterData->HeroLostThunderDuel) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (PMasterData->ThunderDuelResolved) {
                if (IsActiveThreadTerminating()) return;
                HeroHasDefeatedThunder = true;
                if (Game->IsQuestActive("V_HeroDuel")) {
                    if (IsActiveThreadTerminating()) return;
                    Game->DeactivateQuest("V_HeroDuel", 0);
                }
                return;
            }
        }
        if (IsActiveThreadTerminating()) return;
        if (PMasterData->HeroWonThunderDuel) { if (IsActiveThreadTerminating()) return; HeroHasDefeatedThunder = true; }
        else { if (IsActiveThreadTerminating()) return; HeroHasLostToThunder = true; }
    }
};
static bool Run(long running, int outcome, bool completed, const char* expect, bool won, bool lost)
{
    g_log[0] = 0;
    CMasterScriptData data; data.HeroWonThunderDuel = data.HeroLostThunderDuel = data.ThunderDuelResolved = false; data.outcome = outcome;
    CGameScriptInterface game; game.data = &data; game.active = false; game.completed = completed; game.frame = 0;
    CV_MayorsInvitationScript s; s.Game = &game; s.PMasterData = &data; s.HeroHasDefeatedThunder = s.HeroHasLostToThunder = false;
    s.MonitorThunderHeroDuelRunning = running;
    s.MonitorThunderHeroDuel();
    return std::strcmp(g_log, expect) == 0 && s.HeroHasDefeatedThunder == won && s.HeroHasLostToThunder == lost;
}
int main()
{
    bool ok = Run(1, 0, false, "", false, false)
        && Run(0, 0, false, "OBJECTIVE_07;BowerstoneJail;activate;V_HeroDuel;", true, false)
        && Run(0, 1, true, "OBJECTIVE_07;BowerstoneJail;", false, true)
        && Run(0, 2, false, "OBJECTIVE_07;BowerstoneJail;activate;V_HeroDuel;deactivate;V_HeroDuel;", true, false);
    if (!ok) { std::printf("00eb70f0_TEST FAIL\n"); return 1; }
    std::printf("00eb70f0_TEST PASS\n");
    return 0;
}