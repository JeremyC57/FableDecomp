// Behaviour test for NScript::CV_HeroDuelScript::Main (0x00E987C0).
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CMaster { bool DefeatedThunder, LostToThunder; };
struct CGame {
    int loadedAfter;       // IsLevelLoaded("BowerstoneJail") becomes true after this many frames
    int questFrames;       // a competing quest stays active for this many frames once seen
    int frames;
    bool sawQuest;
    void NewScriptFrame() { ++frames; if (questFrames > 0) --questFrames; Log("frame"); }
    bool IsLevelLoaded(const char*) { return frames >= loadedAfter; }
    bool IsQuestActive(const char* q) { return questFrames > 0 && std::strcmp(q, "Q_HangingTreeGood") == 0; }
    void SetTeleportingAsActive(bool b) { Log(b ? "tele1" : "tele0"); }
    void PostAddScriptedEntities() { Log("gpost"); }
    void RemoveQuestCardFromGuild(const char* q) { Log("remove"); Log(q); }
    void SetQuestCardObjective(const char* a, const char* b, const char* c, const char*) { Log("obj"); Log(a); Log(b); Log(c); }
};
struct CV_HeroDuelScript {
    CGame* Game; CMaster* MasterScriptData; int termAfter; int termCalls;
    bool IsActiveThreadTerminating() { return ++termCalls > termAfter; }
    void CleanUpHeroDuel() { Log("cleanup"); }
    void Bind(const char* n, int flags) { Log("bind"); Log(n); Log(flags ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Spawn(const char* n) { Log("spawn"); Log(n); }
    bool AnyQuest() { return Game->IsQuestActive("Q_AmbushTraders") || Game->IsQuestActive("Q_HangingTreeEvil") ||
                             Game->IsQuestActive("Q_HangingTreeGood") || Game->IsQuestActive("Q_EndGameFocalSites") ||
                             Game->IsQuestActive("Q_MinionClifftopChase"); }
    void Main()
    {
        if (MasterScriptData->DefeatedThunder || MasterScriptData->LostToThunder) {
            if (IsActiveThreadTerminating()) return;
            Game->SetTeleportingAsActive(false);
            CleanUpHeroDuel();
        }
        Bind("Thunder", 1);
        Bind("Spectator", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->RemoveQuestCardFromGuild("Q_HangingTreeEvil");
        Game->RemoveQuestCardFromGuild("Q_HangingTreeGood");
        Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_07", "BowerstoneJail", "");
        while (!Game->IsLevelLoaded("BowerstoneJail")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (AnyQuest()) {
                if (IsActiveThreadTerminating()) return;
                while (AnyQuest()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                while (Game->IsLevelLoaded("BowerstoneJail")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
            }
        }
        if (IsActiveThreadTerminating()) return;
        Spawn("WatchForLeaving"); Spawn("ShowEnemyHealth"); Spawn("WatchForHeroEnteringCave"); Spawn("DoMission");
    }
};
static const char* kHead = "bind;Thunder;1;bind;Spectator;0;post;gpost;remove;Q_HangingTreeEvil;remove;Q_HangingTreeGood;"
                           "obj;V_MayorsInvitation;TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_07;BowerstoneJail;";
static const char* kSpawn = "spawn;WatchForLeaving;spawn;ShowEnemyHealth;spawn;WatchForHeroEnteringCave;spawn;DoMission;";
static bool Run(bool won, bool lost, int loadedAfter, int quest, int term, const char* mid, bool spawned)
{
    g_log[0] = 0;
    CMaster m = { won, lost };
    CGame g = { loadedAfter, quest, 0, false };
    CV_HeroDuelScript s = { &g, &m, term, 0 };
    s.Main();
    char expect[4096]; expect[0] = 0;
    if ((won || lost) && term == 0) return g_log[0] == 0;    // terminating before the clean-up: nothing runs
    if (won || lost) std::strcat(expect, "tele0;cleanup;");
    std::strcat(expect, kHead);
    std::strcat(expect, mid);
    if (spawned) std::strcat(expect, kSpawn);
    if (std::strcmp(g_log, expect) != 0) { std::printf("got  %s\nwant %s\n", g_log, expect); return false; }
    return true;
}
int main()
{
    bool ok = Run(false, false, 0, 0, 100, "", true)
        && Run(true, false, 2, 0, 100, "frame;frame;", true)
        && Run(false, true, 1, 0, 100, "frame;", true)
        && Run(false, false, 3, 0, 1, "frame;frame;", false)
        && Run(true, true, 3, 0, 0, "", false)
        && Run(false, false, 5, 2, 100, "frame;frame;frame;frame;frame;", true);
    if (!ok) { std::printf("00e987c0_TEST FAIL\n"); return 1; }
    std::printf("00e987c0_TEST PASS\n");
    return 0;
}