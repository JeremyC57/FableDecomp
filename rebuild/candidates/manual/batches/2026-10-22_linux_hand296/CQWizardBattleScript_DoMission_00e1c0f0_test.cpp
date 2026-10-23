#include <cstdio>
#include <cstring>
#include <string>
// Behaviour test for CQ_WizardBattleScript::DoMission (0x00E1C0F0): waits for HookCoast, sets the objective, hides
// CS_KEY, limbos the village, spawns two threads, records two positions, then runs the phases unless the mission failed.
struct CScriptThing { int Id; };
struct CGSI {
    int Frames; int LoadFrame; char Objective[64]; float KeyAlpha; bool Limbo; char Log[128];
    void NewScriptFrame() { ++Frames; }
    bool IsLevelLoaded(const char* l) { return Frames >= LoadFrame && !std::strcmp(l, "HookCoast"); }
    std::string GetActiveQuestName() { return "Q_Wizard"; }
    void SetQuestCardObjective(const std::string& q, const char* o, const char* r1, const char* r2)
    { if (q == "Q_Wizard" && !std::strcmp(r1, "HookCoast") && !std::strcmp(r2, "HookCoast")) std::strcpy(Objective, o); }
    CScriptThing GetThingWithScriptName(const char* n)
    {
        CScriptThing t; t.Id = !std::strcmp(n, "TopOfLighthouse") ? 10 : !std::strcmp(n, "MazeTeleportPos3") ? 11 : !std::strcmp(n, "CS_KEY") ? 12 : 13;
        return t;
    }
    void EntitySetAlpha(const CScriptThing& t, float a, bool) { if (t.Id == 12) KeyAlpha = a; }
    void SetVillageLimbo(const CScriptThing& t, bool l) { if (t.Id == 13) Limbo = l; }
};
struct CQ_WizardBattleScript {
    CGSI* GSI; bool MissionFailed; bool MazeDefeated; CScriptThing TopOfLighthouse; CScriptThing MiddleOfLighthouse; bool FailInPhaseOne;
    bool IsActiveThreadTerminating() { return GSI->Frames > 30; }
    void Spawn(const char* name) { std::strcat(GSI->Log, name); std::strcat(GSI->Log, ";"); }
    void PhaseOne() { std::strcat(GSI->Log, "one;"); if (FailInPhaseOne) MissionFailed = true; }
    void PhaseTwo() { std::strcat(GSI->Log, "two;"); }
    void EndMission() { std::strcat(GSI->Log, "end;"); }
    void DoMission()
    {
        while (!GSI->IsLevelLoaded("HookCoast")) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_WIZARD_BATTLE_OBJECTIVE_01", "HookCoast", "HookCoast");
        CScriptThing key = GSI->GetThingWithScriptName("CS_KEY");
        GSI->EntitySetAlpha(key, 0.0f, true);
        CScriptThing village = GSI->GetThingWithScriptName("HookCoastVillage");
        GSI->SetVillageLimbo(village, true);
        Spawn("WatchForLeaving");
        Spawn("CutsceneScreamers");
        TopOfLighthouse = GSI->GetThingWithScriptName("TopOfLighthouse");
        MiddleOfLighthouse = GSI->GetThingWithScriptName("MazeTeleportPos3");
        PhaseOne();
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (MissionFailed) return;
        PhaseTwo();
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (MissionFailed) return;
        MazeDefeated = true;
        EndMission();
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.LoadFrame = 3; gsi.KeyAlpha = 1.0f;
    CQ_WizardBattleScript s; std::memset(&s, 0, sizeof s); s.GSI = &gsi;
    s.DoMission();
    bool ok = gsi.Frames == 5 && s.MazeDefeated && !std::strcmp(gsi.Log, "WatchForLeaving;CutsceneScreamers;one;two;end;")
              && !std::strcmp(gsi.Objective, "TEXT_QUEST_WIZARD_BATTLE_OBJECTIVE_01") && gsi.KeyAlpha == 0.0f && gsi.Limbo
              && s.TopOfLighthouse.Id == 10 && s.MiddleOfLighthouse.Id == 11;
    CGSI g2; std::memset(&g2, 0, sizeof g2);
    CQ_WizardBattleScript f; std::memset(&f, 0, sizeof f); f.GSI = &g2; f.FailInPhaseOne = true;
    f.DoMission();
    ok = ok && !f.MazeDefeated && !std::strcmp(g2.Log, "WatchForLeaving;CutsceneScreamers;one;");
    if (!ok) { std::printf("00e1c0f0_TEST FAIL %d %s\n", gsi.Frames, gsi.Log); return 1; }
    std::printf("00e1c0f0_TEST PASS\n");
    return 0;
}
