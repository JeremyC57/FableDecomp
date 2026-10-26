#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingWoodsMeleeScript::Main -- initialise, wait for GuildWoods, bind ScorpionHome, post
// the scripted entities, spawn WatchForTermination and DoMission, then succeed once no scorpion is left alive
static int g_frames = 0, g_endAt = 1000, g_loadedAt = 2, g_deadAt = 5, g_bound = 0, g_spawned = 0, g_posted = 0, g_inits = 0;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "GuildWoods") == 0 && g_frames >= g_loadedAt; }
    void PostAddScriptedEntities() const { ++g_posted; }
};
struct CQ_GuildTrainingWoodsMeleeScript {
    CGameScriptInterface* GSI; bool ScorpionsAlive, MissionSucceeded;
    bool IsActiveThreadTerminating() { if (g_frames >= g_deadAt) ScorpionsAlive = false; return g_frames >= g_endAt; }
    void InitialiseVariables() { ++g_inits; ScorpionsAlive = true; MissionSucceeded = false; }
    void AddEntityScriptBinding(const char* n) { if (std::strcmp(n, "ScorpionHome") == 0) ++g_bound; }
    void PostAddScriptedEntities() { ++g_posted; }
    void SpawnFunction(const char*) { ++g_spawned; }
    void Main();
};
void CQ_GuildTrainingWoodsMeleeScript::Main()
{
    InitialiseVariables();
    while (!GSI->IsLevelLoaded("GuildWoods")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding("ScorpionHome");
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction("WatchForTermination");
    SpawnFunction("DoMission");
    while (ScorpionsAlive) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    MissionSucceeded = true;
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingWoodsMeleeScript s; s.GSI = &game;
    s.Main();
    bool ok = g_inits == 1 && g_bound == 1 && g_posted == 2 && g_spawned == 2 && s.MissionSucceeded && g_frames == 5;
    g_frames = 0; g_bound = 0; g_endAt = 1;
    s.Main();                                           // thread ends before GuildWoods loads
    ok = ok && g_inits == 2 && g_bound == 0 && !s.MissionSucceeded;
    if (!ok) { std::printf("00d66620_TEST FAIL\n"); return 1; }
    std::printf("00d66620_TEST PASS\n");
    return 0;
}