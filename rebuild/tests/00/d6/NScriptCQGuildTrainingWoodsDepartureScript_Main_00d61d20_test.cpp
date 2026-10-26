#include <cstdio>
#include <cstring>
// behaviour test: CQ_GuildTrainingWoodsDepartureScript::Main -- reset flags, wait for GuildWoods, bind three entity
// scripts, post the scripted entities, spawn two threads, set the quest card and create the maze rival hero
static int g_frames = 0, g_endAt = 1000, g_loadedAt = 2, g_bound = 0, g_spawned = 0, g_posted = 0, g_card = 0, g_created = 0;
class C3DVector { public: float X, Y, Z; };
class CScriptThing { public: C3DVector Pos; const C3DVector& GetPos() const { return Pos; } };
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool IsLevelLoaded(const char* n) const { return std::strcmp(n, "GuildWoods") == 0 && g_frames >= g_loadedAt; }
    void PostAddScriptedEntities() const { ++g_posted; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.Pos.X = std::strcmp(n, "MazeCreationMarker") == 0 ? 7.0f : 0.0f; t.Pos.Y = 0; t.Pos.Z = 0; return t; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* name, bool a) const
    {
        if (std::strcmp(def, "CREATURE_RIVAL_HERO_MAZE_TUTORIAL") == 0 && pos.X == 7.0f && std::strcmp(name, "FinalMaze") == 0 && !a) ++g_created;
        CScriptThing t; t.Pos = pos; return t;
    }
    void SetQuestCardObjective(const char* q, const char* t, const char* r, const char* l) const
    {
        if (std::strcmp(q, "Q_GuildTraining") == 0 && std::strcmp(t, "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12") == 0 && !*r && !*l) ++g_card;
    }
};
struct CQ_GuildTrainingWoodsDepartureScript {
    CGameScriptInterface* GSI; bool MissionSucceeded, MissionFailed, MissionOver; long DepartureMissionPoint;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void InitialiseVariables() { MissionSucceeded = false; MissionFailed = false; MissionOver = false; }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() { ++g_posted; }
    void SpawnFunction(const char*) { ++g_spawned; }
    void Main();
};
void CQ_GuildTrainingWoodsDepartureScript::Main()
{
    InitialiseVariables();
    while (!GSI->IsLevelLoaded("GuildWoods")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding("ArtifactThief");
    AddEntityScriptBinding("FinalMaze");
    AddEntityScriptBinding("ScorpionHome");
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    SpawnFunction("WatchForTermination");
    SpawnFunction("DoMission");
    DepartureMissionPoint = 0;
    GSI->SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "");
    GSI->CreateCreature("CREATURE_RIVAL_HERO_MAZE_TUTORIAL", GSI->GetThingWithScriptName("MazeCreationMarker").GetPos(), "FinalMaze", false);
}
int main()
{
    CGameScriptInterface game; CQ_GuildTrainingWoodsDepartureScript s; s.GSI = &game; s.MissionFailed = true; s.DepartureMissionPoint = 9;
    s.Main();
    bool ok = g_bound == 3 && g_posted == 2 && g_spawned == 2 && g_card == 1 && g_created == 1 && s.DepartureMissionPoint == 0 && !s.MissionFailed;
    g_frames = 0; g_bound = 0; g_endAt = 1; s.DepartureMissionPoint = 9;
    s.Main();                                           // thread ends before GuildWoods loads
    ok = ok && g_bound == 0 && s.DepartureMissionPoint == 9 && g_created == 1;
    if (!ok) { std::printf("00d61d20_TEST FAIL\n"); return 1; }
    std::printf("00d61d20_TEST PASS\n");
    return 0;
}