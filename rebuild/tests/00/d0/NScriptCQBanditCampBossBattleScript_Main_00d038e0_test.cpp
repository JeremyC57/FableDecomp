// Behaviour test for NScript::CQ_BanditCampBossBattleScript::Main (0x00D038E0): bind the fourteen entity scripts in retail order
// (BCGameMaster the one flag-0 binding), post the scripted entities to the script and the game, spawn the seven quest threads and
// cache the Twinblade / Theresa 1 / Theresa 2 cut-scene music sets (48, 49, 50). A log model of the call sequence.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Binding { const char* Name; long Flag; };
static Binding g_bind[20]; static int g_nbind;
static long g_music[4]; static int g_nmusic;
struct Quest {
    void Bind(const char* name, long flag) { Binding b = { name, flag }; g_bind[g_nbind++] = b; }
    void PostAddScriptedEntities() { Log("post_script"); }
    void GamePostAddScriptedEntities() { Log("post_game"); }
    void Spawn(const char* name) { Log(name); }
    void CacheMusicSet(long set) { g_music[g_nmusic++] = set; }
    void Main()
    {
        Bind("Gate2Guard1", 1); Bind("BCGameMaster", 0); Bind("AssassinMarker", 1); Bind("Assassin1", 1); Bind("Assassin2", 1);
        Bind("Assassin3", 1); Bind("Gate3Guard", 1); Bind("CampHostage", 1); Bind("CampHostage2", 1); Bind("CampHostageDoor", 1);
        Bind("CampHostageGuard", 1); Bind("BanditForger", 1); Bind("BanditKing", 1); Bind("CROWDBANDITS", 1);
        PostAddScriptedEntities();
        GamePostAddScriptedEntities();
        Spawn("BanditKingMissionProcess"); Spawn("WatchForBanditCampGates"); Spawn("ProcessHostageCutscenes"); Spawn("WatchForTermination");
        Spawn("CheckForFirstAreaMassacre"); Spawn("CheckForSecondAreaMassacre"); Spawn("CheckForSecondAreaDoorHelp");
        CacheMusicSet(48); CacheMusicSet(49); CacheMusicSet(50);
    }
};
int main()
{
    Quest q;
    q.Main();
    int flags1 = 0;
    for (int i = 0; i < g_nbind; ++i) flags1 += g_bind[i].Flag;
    bool ok = !std::strcmp(g_log, "post_script;post_game;BanditKingMissionProcess;WatchForBanditCampGates;ProcessHostageCutscenes;"
                                  "WatchForTermination;CheckForFirstAreaMassacre;CheckForSecondAreaMassacre;CheckForSecondAreaDoorHelp;")
        && g_nbind == 14 && flags1 == 13 && g_bind[1].Flag == 0 && !std::strcmp(g_bind[13].Name, "CROWDBANDITS")
        && g_nmusic == 3 && g_music[0] == 48 && g_music[1] == 49 && g_music[2] == 50;
    std::printf(ok ? "00d038e0_TEST PASS\n" : "00d038e0_TEST FAIL\n");
    return ok ? 0 : 1;
}