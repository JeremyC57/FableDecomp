// Behaviour test for NScript::CV_IntroductionToTrophiesScript::Main 0x00E9C4B0: the four entity-script bindings in order,
// both PostAddScriptedEntities calls, the three spawned watcher threads (each with an empty region), the QuestComplete
// wait (one NewScriptFrame per pass) and the closing ActivateQuest("V_TalentlessBard") + DeactivateQuestLater, plus the
// early exits when the thread terminates inside the wait or right after it.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Game {
    int framesUntilComplete;
    bool* questComplete;
    void NewScriptFrame() { Log("Frame"); if (framesUntilComplete > 0 && --framesUntilComplete == 0) *questComplete = true; }
    void PostAddScriptedEntities() { Log("GamePost"); }
    void ActivateQuest(const char* name) { Log("Activate"); Log(name); }
    void DeactivateQuestLater(const char* name, unsigned int frames) { if (frames == 0) { Log("Deactivate"); Log(name); } }
    const char* GetActiveQuestName() const { return "V_IntroductionToTrophies"; }
};
struct Script {
    Game* Game_;
    bool QuestComplete;
    int terminateAt, calls;
    bool IsActiveThreadTerminating() { ++calls; return calls == terminateAt; }
    void AddEntityScriptBinding(const char* name, unsigned int flags) { if (flags == 0) Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void SpawnFunction(const char* name, const char* region) { if (region[0] == 0) { Log("Spawn"); Log(name); } }
    void Main()
    {
        AddEntityScriptBinding("BriarRose", 0);
        AddEntityScriptBinding("TrophiesIntroWatcher", 0);
        AddEntityScriptBinding("TrophiesBriarTrophy", 0);
        AddEntityScriptBinding("TrophiesIntroTable", 0);
        PostAddScriptedEntities();
        Game_->PostAddScriptedEntities();
        SpawnFunction("WatchForInsideTavern", "");
        SpawnFunction("WatchForCrowdVolume", "");
        SpawnFunction("WatchForQuestComplete", "");
        while (!QuestComplete) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game_->ActivateQuest("V_TalentlessBard");
        Game_->DeactivateQuestLater(Game_->GetActiveQuestName(), 0);
    }
};
static bool Run(int framesUntilComplete, bool already, int terminateAt, const char* expect)
{
    g_log[0] = 0;
    Script s; Game g;
    s.Game_ = &g; s.QuestComplete = already; s.terminateAt = terminateAt; s.calls = 0;
    g.framesUntilComplete = framesUntilComplete; g.questComplete = &s.QuestComplete;
    s.Main();
    if (std::strcmp(g_log, expect) != 0) { std::printf("got %s\n", g_log); return false; }
    return true;
}
int main()
{
    const char* head = "BriarRose;TrophiesIntroWatcher;TrophiesBriarTrophy;TrophiesIntroTable;Post;GamePost;"
                       "Spawn;WatchForInsideTavern;Spawn;WatchForCrowdVolume;Spawn;WatchForQuestComplete;";
    const char* tail = "Activate;V_TalentlessBard;Deactivate;V_IntroductionToTrophies;";
    char full[512], now[512], inwait[512], after[512];
    std::sprintf(full, "%sFrame;Frame;%s", head, tail);
    std::sprintf(now, "%s%s", head, tail);
    std::sprintf(inwait, "%sFrame;", head);
    std::sprintf(after, "%sFrame;Frame;", head);
    if (!Run(2, false, 0, full) || !Run(0, true, 0, now) || !Run(3, false, 1, inwait) || !Run(2, false, 3, after)) {
        std::printf("00e9c4b0_TEST FAIL\n");
        return 1;
    }
    std::printf("00e9c4b0_TEST PASS\n");
    return 0;
}