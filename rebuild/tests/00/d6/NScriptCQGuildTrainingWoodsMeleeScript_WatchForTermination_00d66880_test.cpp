#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingWoodsMeleeScript::WatchForTermination -- when the mission ends, fail or complete
// the quest, point the card at the guild objective, wait five frames, have the guildmaster call the hero back,
// record the scorpion cutscene on success, then deactivate the quest once HeroGuildComplex is loaded
static int g_frames = 0, g_endAt = 1000, g_failed = 0, g_completed = 0, g_card = 0, g_lines = 0, g_deactivated = 0, g_guildAt = 9;
static int g_lineFrame = -1;
struct SMasterData { bool ScorpionsDestroyedCutscenePlayed; };
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    void SetQuestAsFailed() const { ++g_failed; }
    void SetQuestAsCompleted() const { ++g_completed; }
    void SetQuestCardObjective(const char* q, const char* t) const
    { if (!std::strcmp(q, "Q_GuildTraining") && !std::strcmp(t, "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02")) ++g_card; }
    void AddLineToConversation(const char* t) const { if (!std::strcmp(t, "TEXT_QST_028_GUILDSEAL_COME_BACK")) { ++g_lines; g_lineFrame = g_frames; } }
    bool IsLevelLoaded(const char* n) const { return !std::strcmp(n, "HeroGuildComplex") && g_frames >= g_guildAt; }
    void DeactivateQuestLater(const char* q, long d) const { if (!std::strcmp(q, "Q_GuildTrainingWoodsMelee") && d == 0) ++g_deactivated; }
};
struct CQ_GuildTrainingWoodsMeleeScript {
    CGameScriptInterface* Game; SMasterData* PMasterData; bool MissionFailed, MissionSucceeded;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void WatchForTermination();
};
void CQ_GuildTrainingWoodsMeleeScript::WatchForTermination()
{
    while (!MissionFailed && !MissionSucceeded) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (MissionFailed) Game->SetQuestAsFailed();
    else Game->SetQuestAsCompleted();
    Game->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02");
    for (int i = 0; i < 5; ++i) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    Game->AddLineToConversation("TEXT_QST_028_GUILDSEAL_COME_BACK");
    if (MissionSucceeded) PMasterData->ScorpionsDestroyedCutscenePlayed = true;
    while (!Game->IsLevelLoaded("HeroGuildComplex")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0);
}
int main()
{
    CGameScriptInterface game; SMasterData md; md.ScorpionsDestroyedCutscenePlayed = false;
    CQ_GuildTrainingWoodsMeleeScript s; s.Game = &game; s.PMasterData = &md; s.MissionFailed = false; s.MissionSucceeded = true;
    s.WatchForTermination();
    bool ok = g_completed == 1 && g_failed == 0 && g_card == 1 && g_lines == 1 && g_lineFrame == 5
           && md.ScorpionsDestroyedCutscenePlayed && g_deactivated == 1 && g_frames == 9;
    g_frames = 0; md.ScorpionsDestroyedCutscenePlayed = false; s.MissionSucceeded = false; s.MissionFailed = true;
    s.WatchForTermination();                            // failed: no cutscene flag
    ok = ok && g_failed == 1 && g_completed == 1 && !md.ScorpionsDestroyedCutscenePlayed && g_deactivated == 2;
    if (!ok) { std::printf("00d66880_TEST FAIL\n"); return 1; }
    std::printf("00d66880_TEST PASS\n");
    return 0;
}