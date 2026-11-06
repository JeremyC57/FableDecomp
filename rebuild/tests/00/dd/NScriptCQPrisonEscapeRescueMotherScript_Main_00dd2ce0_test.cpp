// Behaviour test for NScript::CQ_PrisonEscapeRescueMotherScript::Main (0x00DD2CE0): switch teleporting off, set the quest card
// objective (TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_02 at PrisonCourtyard) for the active quest, bind the fifteen scripted-entity
// names in order (the four key doors share the CPrisonKeyDoor allocator; PrisonGuard, CellDoorHero, CellDoor and the key doors
// with flag 0, the rest with flag 1), post the scripted entities to the script and the game, add logbook entry 220 and spawn the
// four threads. A log model of the call sequence.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Binding { const char* Name; const char* Alloc; long Flag; };
static Binding g_bind[16]; static int g_nbind;
struct Quest {
    const char* ActiveQuest;
    void SetTeleportingAsActive(bool on) { Log(on ? "teleport_on" : "teleport_off"); }
    void SetQuestCardObjective(const char* quest, const char* text, const char* region, const char* region2)
    { Log(quest); Log(text); Log(region); if (*region2) Log("BAD"); }
    void Bind(const char* name, const char* alloc, long flag) { Binding b = { name, alloc, flag }; g_bind[g_nbind++] = b; }
    void PostAddScriptedEntities() { Log("post_script"); }
    void GamePostAddScriptedEntities() { Log("post_game"); }
    void AddLogbookStoryEntry(long e) { if (e == 220) Log("logbook220"); }
    void Spawn(const char* name) { Log(name); }
    void Main()
    {
        SetTeleportingAsActive(false);
        SetQuestCardObjective(ActiveQuest, "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_02", "PrisonCourtyard", "");
        Bind("PrisonGuard", "CPrisonGuard", 0);
        Bind("PrisonerAccomplice", "CPrisonerAccomplice", 1);
        Bind("Prisoner", "CPrisoner", 1);
        Bind("PrisonEscapeGuard", "CPrisonEscapeGuard", 1);
        Bind("MotherTorture", "CMotherTorture", 1);
        Bind("GuardMotherEscape", "CGuardMotherEscape", 1);
        Bind("InventoryChest", "CInventoryChest", 1);
        Bind("TortureDoorPassage", "CTortureDoorPassage", 1);
        Bind("TortureDoorCourtyard", "CTortureDoorCourtyard", 1);
        Bind("CellDoorHero", "CCellDoorHero", 0);
        Bind("CellDoor", "CCellDoor", 0);
        Bind("StoreRoomDoor", "CPrisonKeyDoor", 0);
        Bind("TortureChamberDoor", "CPrisonKeyDoor", 0);
        Bind("BarracksDoor", "CPrisonKeyDoor", 0);
        Bind("PrisonKeyDoor", "CPrisonKeyDoor", 0);
        PostAddScriptedEntities();
        GamePostAddScriptedEntities();
        AddLogbookStoryEntry(220);
        Spawn("WatchHeroEscapeStatus"); Spawn("SplittingUpCutscene"); Spawn("PrisonLifeIntro"); Spawn("ProcessRaces");
    }
};
int main()
{
    Quest q = { "Q_PrisonEscape" };
    q.Main();
    int flags1 = 0, keydoors = 0;
    for (int i = 0; i < g_nbind; ++i) { flags1 += g_bind[i].Flag; if (!std::strcmp(g_bind[i].Alloc, "CPrisonKeyDoor")) ++keydoors; }
    bool ok = !std::strcmp(g_log, "teleport_off;Q_PrisonEscape;TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_02;PrisonCourtyard;post_script;post_game;"
                                  "logbook220;WatchHeroEscapeStatus;SplittingUpCutscene;PrisonLifeIntro;ProcessRaces;")
        && g_nbind == 15 && flags1 == 8 && keydoors == 4 && !std::strcmp(g_bind[0].Name, "PrisonGuard")
        && !std::strcmp(g_bind[14].Name, "PrisonKeyDoor") && g_bind[9].Flag == 0 && g_bind[8].Flag == 1;
    std::printf(ok ? "00dd2ce0_TEST PASS\n" : "00dd2ce0_TEST FAIL\n");
    return ok ? 0 : 1;
}