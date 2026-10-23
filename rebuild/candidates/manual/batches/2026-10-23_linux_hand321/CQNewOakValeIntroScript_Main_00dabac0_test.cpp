#include <cstdio>
#include <cstring>
// behaviour test: CQ_NewOakValeIntroScript::Main -- sixteen entity bindings, close the post-attack quest only when the
// attack is over (and the thread is alive), set the active quest's first objective, spawn StartBarrelTimer, run DoMission
static int g_bound, g_deact, g_obj, g_spawned, g_mission;
static bool g_term;
struct CGameScriptInterface {
    void PostAddScriptedEntities() const {}
    void DeactivateQuest(const char* q, unsigned long f) const { if (!std::strcmp(q, "Q__OakValeIntro_PostAttack") && f == 0) ++g_deact; }
    void SetQuestCardObjective(const char* q, const char* t, const char* r, const char* l) const
    {
        if (!std::strcmp(q, "Q_OakValeIntro") && !std::strcmp(t, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01") && !*r && !*l) ++g_obj;
    }
    const char* GetActiveQuestName() const { return "Q_OakValeIntro"; }
};
struct CQ_NewOakValeIntroScript {
    CGameScriptInterface* Game; bool AttackOver;
    bool IsActiveThreadTerminating() { return g_term; }
    void AddEntityScriptBinding(const char* n) { if (std::strstr(n, "OVI_")) ++g_bound; }
    void PostAddScriptedEntities() {}
    void SpawnFunction(const char* n) { if (!std::strcmp(n, "StartBarrelTimer")) ++g_spawned; }
    void DoMission() { ++g_mission; }
    void Main();
};
void CQ_NewOakValeIntroScript::Main()
{
    static const char* const names[16] = { "NOVI_LiveFather", "NOVI_Theresa", "NOVI_Guard", "NOVI_Villager", "NOVI_Bully", "NOVI_Victim",
        "NOVI_TeddyGirl", "NOVI_AffairMan", "NOVI_AffairWoman", "NOVI_AffairWife", "NOVI_BookTrader", "NOVI_BarrelMan",
        "NOVI_BarrelThug", "NOVI_Barrel", "NOVI_CreatedBeetle", "OVI_DeadFather" };
    for (int i = 0; i < 16; ++i)
        AddEntityScriptBinding(names[i]);
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (AttackOver) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("Q__OakValeIntro_PostAttack", 0);
    }
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "");
    SpawnFunction("StartBarrelTimer");
    DoMission();
}
int main()
{
    CGameScriptInterface game;
    CQ_NewOakValeIntroScript s; s.Game = &game;
    s.AttackOver = false; g_term = true;
    s.Main();                                           // attack not over: the terminate flag is not consulted
    bool ok = g_bound == 16 && g_deact == 0 && g_obj == 1 && g_spawned == 1 && g_mission == 1;
    s.AttackOver = true; g_term = false;
    s.Main();
    ok = ok && g_bound == 32 && g_deact == 1 && g_obj == 2 && g_mission == 2;
    g_term = true;
    s.Main();                                           // attack over but the thread is ending: stop after the bindings
    ok = ok && g_bound == 48 && g_deact == 1 && g_obj == 2 && g_spawned == 2 && g_mission == 2;
    if (!ok) { std::printf("00dabac0_TEST FAIL\n"); return 1; }
    std::printf("00dabac0_TEST PASS\n");
    return 0;
}
