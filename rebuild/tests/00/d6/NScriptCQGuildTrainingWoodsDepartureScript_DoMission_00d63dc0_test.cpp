#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingWoodsDepartureScript::DoMission -- give the first objective, wait for GuildWoods,
// spawn the WatchForLeaving and TeleportOutHero threads, then (unless already failed) wait for the mission to end
// and complete the quest
static int g_frames = 0, g_endAt = 1000, g_loadedAt = 2, g_overAt = 6, g_spawned = 0, g_completed = 0, g_objectives = 0;
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    void GiveHeroNewQuestObjective(const char* t, long n) const { if (std::strcmp(t, "first objective") == 0 && n == 1) ++g_objectives; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "GuildWoods") == 0 && g_frames >= g_loadedAt; }
    void SetQuestAsCompleted(bool a, bool b, bool c) const { if (a && b && !c) ++g_completed; }
};
struct CQ_GuildTrainingWoodsDepartureScript {
    CGameScriptInterface* Game; bool MissionFailed, MissionOver, MissionSucceeded;
    bool IsActiveThreadTerminating() { if (g_frames >= g_overAt) MissionOver = true; return g_frames >= g_endAt; }
    void SpawnFunction(const char* name) { if (!std::strcmp(name, "WatchForLeaving") || !std::strcmp(name, "TeleportOutHero")) ++g_spawned; }
    void DoMission();
};
void CQ_GuildTrainingWoodsDepartureScript::DoMission()
{
    Game->GiveHeroNewQuestObjective("first objective", 1);
    while (!Game->IsLevelLoaded("GuildWoods")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    SpawnFunction("WatchForLeaving");
    SpawnFunction("TeleportOutHero");
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    if (MissionFailed) return;
    while (!MissionOver) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestAsCompleted(true, true, false);
    MissionSucceeded = true;
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingWoodsDepartureScript s; s.Game = &game;
    s.MissionFailed = false; s.MissionOver = false; s.MissionSucceeded = false;
    s.DoMission();
    bool ok = g_objectives == 1 && g_spawned == 2 && g_completed == 1 && s.MissionSucceeded && g_frames == 6;
    g_frames = 0; g_spawned = 0; g_completed = 0; s.MissionSucceeded = false; s.MissionOver = false; s.MissionFailed = true;
    s.DoMission();                                      // already failed: threads spawned, nothing completed
    ok = ok && g_spawned == 2 && g_completed == 0 && !s.MissionSucceeded;
    if (!ok) { std::printf("00d63dc0_TEST FAIL\n"); return 1; }
    std::printf("00d63dc0_TEST PASS\n");
    return 0;
}