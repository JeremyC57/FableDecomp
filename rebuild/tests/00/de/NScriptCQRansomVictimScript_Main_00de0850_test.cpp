// Behaviour test for NScript::CQ_RansomVictimScript::Main (0x00DE0850): after the ten bindings (NPCComment flag 0) and the three
// threads, wait for Knothole Glade, set up gossip, open KG_Gate and set objective 1; wait for the quest start screen, activate the two
// start sub-quests and set objective 2; wait for the victim's rescue, deactivate the nature sub-quest, switch off the Witchwood 2-4
// generators and set objective 3; activate the natures-revenge sub-quest unless the ransommer is already dead, wait for his death,
// then activate the revenge sub-quest, deactivate natures-revenge and make RansomRevenge the active enemy. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    bool QuestStartScreened, RescuedVictim, RansommerDied;
    const char* ActiveEnemyName;
    int Frames, Bindings, Flags;
    bool IsRegionLoaded(const char*) { return Frames >= 1; }
    void NewScriptFrame()
    {
        ++Frames;
        if (Frames == 2) QuestStartScreened = true;
        if (Frames == 3) RescuedVictim = true;
        if (Frames == 5) RansommerDied = true;
    }
    bool IsActiveThreadTerminating() { return false; }
    void Main()
    {
        for (int i = 0; i < 10; ++i) { ++Bindings; Flags += i == 9 ? 0 : 1; }
        Log("threads");
        while (!IsRegionLoaded("KnotholeGlade")) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("gossip"); Log("open_gate"); Log("obj1");
        while (!QuestStartScreened) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("Q_RansomVictimStarted"); Log("Q_RansomVictimStartedNature"); Log("obj2");
        while (!RescuedVictim) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("-Q_RansomVictimStartedNature"); Log("gen_off_2"); Log("gen_off_3"); Log("gen_off_4"); Log("obj3");
        if (!RansommerDied) { if (IsActiveThreadTerminating()) return; Log("Q_RansomVictimNaturesRevenge"); }
        while (!RansommerDied) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("Q_RansomVictimRevenge"); Log("-Q_RansomVictimNaturesRevenge");
        ActiveEnemyName = "RansomRevenge";
    }
};
int main()
{
    Quest q = { false, false, false, "", 0, 0, 0 };
    q.Main();
    bool ok = !std::strcmp(g_log, "threads;gossip;open_gate;obj1;Q_RansomVictimStarted;Q_RansomVictimStartedNature;obj2;"
                                  "-Q_RansomVictimStartedNature;gen_off_2;gen_off_3;gen_off_4;obj3;Q_RansomVictimNaturesRevenge;"
                                  "Q_RansomVictimRevenge;-Q_RansomVictimNaturesRevenge;")
        && q.Frames == 5 && q.Bindings == 10 && q.Flags == 9 && !std::strcmp(q.ActiveEnemyName, "RansomRevenge");
    std::printf(ok ? "00de0850_TEST PASS\n" : "00de0850_TEST FAIL\n");
    return ok ? 0 : 1;
}