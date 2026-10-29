// Behaviour test for NScript::CV_TrophyDealerScript::Main 0x00EE6E90: Witchwood2's creature generators off first, the two
// entity-script bindings, both PostAddScriptedEntities calls, V_SingingStones activated without resources, the spawned
// HilightGuildTeleporter thread, the Witchwood1 wait, then the quest card objective and the quest start screen; plus the
// early exits when the thread terminates during or right after the wait.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Game {
    int framesUntilLoaded;
    void NewScriptFrame() { Log("Frame"); }
    bool IsRegionLoaded(const char* name) { if (std::strcmp(name, "Witchwood1") != 0) return false; return framesUntilLoaded-- <= 0; }
    void PostAddScriptedEntities() { Log("GamePost"); }
    void SetCreatureGeneratorsEnabled(const char* region, bool enabled) { char b[64]; std::sprintf(b, "Generators(%s,%d)", region, enabled); Log(b); }
    void ActivateQuestWithoutLoadingResources(const char* name) { Log("ActivateNoRes"); Log(name); }
    const char* GetActiveQuestName() const { return "V_TrophyDealer"; }
    void SetQuestCardObjective(const char* quest, const char* text, const char* region1, const char* region2)
    { char b[160]; std::sprintf(b, "Objective(%s,%s,%s,%s)", quest, text, region1, region2); Log(b); }
    void KickOffQuestStartScreen(const char* name, bool a, bool b2) { char b[64]; std::sprintf(b, "StartScreen(%s,%d,%d)", name, a, b2); Log(b); }
};
struct Script {
    Game* Game_;
    int terminateAt, calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void SpawnFunction(const char* name, const char* region) { if (region[0] == 0) { Log("Spawn"); Log(name); } }
    void Main()
    {
        Game_->SetCreatureGeneratorsEnabled("Witchwood2", false);
        AddEntityScriptBinding("TrophyDealerInCave");
        AddEntityScriptBinding("DemonDoorFace");
        PostAddScriptedEntities();
        Game_->PostAddScriptedEntities();
        Game_->ActivateQuestWithoutLoadingResources("V_SingingStones");
        SpawnFunction("HilightGuildTeleporter", "");
        while (!Game_->IsRegionLoaded("Witchwood1")) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game_->SetQuestCardObjective(Game_->GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", "");
        Game_->KickOffQuestStartScreen("V_TrophyDealer", true, false);
    }
};
static bool Run(int framesUntilLoaded, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    Script s; Game g;
    s.Game_ = &g; s.terminateAt = terminateAt; s.calls = 0; g.framesUntilLoaded = framesUntilLoaded;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    return true;
}
int main()
{
    const char* head = "Generators(Witchwood2,0);TrophyDealerInCave;DemonDoorFace;Post;GamePost;ActivateNoRes;V_SingingStones;"
                       "Spawn;HilightGuildTeleporter;";
    const char* tail = "Objective(V_TrophyDealer,TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02,Witchwood2,);StartScreen(V_TrophyDealer,1,0);";
    char full[512], now[512], inwait[512], after[512];
    std::sprintf(full, "%sFrame;Frame;%s", head, tail);
    std::sprintf(now, "%s%s", head, tail);
    std::sprintf(inwait, "%sFrame;", head);
    std::sprintf(after, "%sFrame;Frame;", head);
    if (!Run(2, 0, full) || !Run(0, 0, now) || !Run(3, 1, inwait) || !Run(2, 3, after)) {
        std::printf("00ee6e90_TEST FAIL\n");
        return 1;
    }
    std::printf("00ee6e90_TEST PASS\n");
    return 0;
}