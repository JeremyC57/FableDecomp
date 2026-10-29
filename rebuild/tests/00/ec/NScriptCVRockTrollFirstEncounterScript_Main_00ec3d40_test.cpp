// Behaviour test for NScript::CV_RockTrollFirstEncounterScript::Main 0x00EC3D40: binding registration order, the
// Witchwood1 wait, the spawned region-exit watcher, the one-shot quest activation, the mission-over wait, the quest-info
// clean-up, the region-message wait and the closing quest calls, plus the early exits when the thread terminates.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Game {
    int framesUntilLoaded, framesUntilMsg, framesUntilOver;
    bool* missionOver;
    void NewScriptFrame() { Log("Frame"); if (framesUntilOver > 0 && --framesUntilOver == 0) *missionOver = true; }
    bool IsRegionLoaded(const char* name) { if (std::strcmp(name, "Witchwood1") != 0) return false; return framesUntilLoaded-- <= 0; }
    bool MsgOnRegionLoaded(char* name) { if (framesUntilMsg-- > 0) return false; std::strcpy(name, "Witchwood2"); return true; }
    void PostAddScriptedEntities() { Log("GamePost"); }
    void ActivateQuest(const char* name) { Log("Activate"); Log(name); }
    void DeactivateQuestLater(const char* name, unsigned int frames) { if (frames == 0) { Log("Deactivate"); Log(name); } }
    void DisplayQuestInfo(bool display) { Log(display ? "Info1" : "Info0"); }
    void RemoveQuestInfoElement(long id) { char b[32]; std::sprintf(b, "RemoveInfo(%ld)", id); Log(b); }
    void SetCreatureGeneratorsEnabled(const char* region, bool enabled) { char b[64]; std::sprintf(b, "Generators(%s,%d)", region, enabled); Log(b); }
    const char* GetActiveQuestName() const { return "Q_ROCKTROLL"; }
};
struct Script {
    Game* game;
    bool MissionOver, Activated;
    long RockTrollHealthBarID;
    int terminateAt, calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void SpawnFunction(const char* name, const char* region) { if (region[0] == 0) { Log("Spawn"); Log(name); } }
    void Main()
    {
        AddEntityScriptBinding("RTFE_Sparrow");
        AddEntityScriptBinding("RTFE_RockTroll");
        AddEntityScriptBinding("M_RTFERockTrollTrigger");
        PostAddScriptedEntities();
        game->PostAddScriptedEntities();
        while (!game->IsRegionLoaded("Witchwood1")) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        SpawnFunction("WatchForRegionExit", "");
        if (!Activated) {
            if (IsActiveThreadTerminating()) return;
            game->ActivateQuest("V_RockTrollFirstEncounter_Activate");
            Activated = true;
        }
        while (!MissionOver) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        game->RemoveQuestInfoElement(RockTrollHealthBarID);
        game->DisplayQuestInfo(false);
        char region[32] = "";
        while (!game->MsgOnRegionLoaded(region)) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        game->SetCreatureGeneratorsEnabled("Witchwood1", true);
        game->DeactivateQuestLater("V_RockTrollFirstEncounter_Activate", 0);
        game->DeactivateQuestLater(game->GetActiveQuestName(), 0);
    }
};
static bool Run(bool activated, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    Script s; Game g;
    s.game = &g; s.MissionOver = false; s.Activated = activated; s.RockTrollHealthBarID = 42; s.terminateAt = terminateAt; s.calls = 0;
    g.framesUntilLoaded = 1; g.framesUntilMsg = 1; g.framesUntilOver = 2; g.missionOver = &s.MissionOver;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    return true;
}
int main()
{
    const char* head = "RTFE_Sparrow;RTFE_RockTroll;M_RTFERockTrollTrigger;Post;GamePost;Frame;";
    char full[512], noact[512];
    std::sprintf(full, "%sSpawn;WatchForRegionExit;Activate;V_RockTrollFirstEncounter_Activate;Frame;RemoveInfo(42);Info0;Frame;"
                 "Generators(Witchwood1,1);Deactivate;V_RockTrollFirstEncounter_Activate;Deactivate;Q_ROCKTROLL;", head);
    std::sprintf(noact, "%sSpawn;WatchForRegionExit;Frame;RemoveInfo(42);Info0;Frame;"
                 "Generators(Witchwood1,1);Deactivate;V_RockTrollFirstEncounter_Activate;Deactivate;Q_ROCKTROLL;", head);
    char early[256];
    std::sprintf(early, "%sSpawn;WatchForRegionExit;", head);
    if (!Run(false, 0, full) || !Run(true, 0, noact) || !Run(false, 1, head) || !Run(false, 3, early)) {
        std::printf("00ec3d40_TEST FAIL\n");
        return 1;
    }
    std::printf("00ec3d40_TEST PASS\n");
    return 0;
}