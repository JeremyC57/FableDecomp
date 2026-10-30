// Behaviour test for NScript::CV_TourGuideScript::Main (0x00EE47B0): bind the guide and the follower, post-add the scripted
// entities (script, then game), spawn WatchForGuideKilled and WatchForClosingTime, wait frame by frame until the quest is
// finished, then deactivate the active quest; a terminating thread returns before the deactivation.
#include <cstdio>
#include <cstring>

static char gLog[512];
static void Log(const char* s) { std::strcat(gLog, s); std::strcat(gLog, ";"); }

struct Game {
    int frames, finishAt;
    bool* finished;
    void PostAddScriptedEntities() { Log("GamePost"); }
    void NewScriptFrame() { ++frames; if (frames == finishAt) *finished = true; }
    const char* GetActiveQuestName() { return "V_TourGuide"; }
    void DeactivateQuestLater(const char* name, unsigned long delay) { if (delay == 0) Log(name); }
};

struct CV_TourGuideScript {
    Game* Game_;
    bool QuestFinished;
    int termAt, checks;
    bool IsActiveThreadTerminating() { ++checks; return checks >= termAt; }
    void AddEntityScriptBinding(const char* name) { Log(name); }
    void PostAddScriptedEntities() { Log("Post"); }
    void SpawnFunction(const char* name) { Log(name); }
    void Main()
    {
        AddEntityScriptBinding("TourGuideGuide");
        AddEntityScriptBinding("TourGuideFollower");
        PostAddScriptedEntities();
        Game_->PostAddScriptedEntities();
        SpawnFunction("WatchForGuideKilled");
        SpawnFunction("WatchForClosingTime");
        while (!QuestFinished) {
            Game_->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game_->DeactivateQuestLater(Game_->GetActiveQuestName(), 0);
    }
};

int main()
{
    CV_TourGuideScript s; Game g;
    s.Game_ = &g; s.QuestFinished = false; s.termAt = 1000; s.checks = 0;
    g.frames = 0; g.finishAt = 3; g.finished = &s.QuestFinished;
    gLog[0] = 0;
    s.Main();
    if (std::strcmp(gLog, "TourGuideGuide;TourGuideFollower;Post;GamePost;WatchForGuideKilled;WatchForClosingTime;V_TourGuide;") != 0
        || g.frames != 3) { std::printf("00ee47b0_TEST FAIL\n"); return 1; }
    s.QuestFinished = false; s.termAt = 2; s.checks = 0; g.frames = 0; g.finishAt = 100; gLog[0] = 0;
    s.Main();
    if (std::strstr(gLog, "V_TourGuide") != 0 || g.frames != 2) { std::printf("00ee47b0_TEST FAIL\n"); return 1; }
    std::printf("00ee47b0_TEST PASS\n");
    return 0;
}