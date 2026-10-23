#include <cstdio>
#include <cstring>
// behaviour test: CQ_OrchardFarmRaidScript::Init -- quest-region pick, state reset, team pairing, and the evil/good branch
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_active = "";
struct Game {
    bool IsQuestActive(const char* q) const { return std::strcmp(q, g_active) == 0; }
    void AddQuestRegion(const char* q, const char* r) const { L(q); L(r); }
    void SetQuestCardObjective(const char* q, const char* t, const char*, const char* r2) const { L(q); L(t); L(r2); }
    void MiniMap(const char* a, const char* b, bool allow) const { L(a); L(b); L(allow ? "1" : "0"); }
};
struct Team { long Carrier; Team* EnemyTeam; };
struct Script {
    Game* G; bool GuardKilled, BanditKilled, Stolen, NoneStolen; long CrateCount; long Timer;
    bool Done, Wrong, Spawned, Succeeded; long Failed; const char* Reasons[4]; long HeroTeam; long Waves;
    const char* Code; bool Met, T1, T2, Cut; Team Teams[2];
    void Init()
    {
        if (G->IsQuestActive("Q_OrchardFarmRaidGood")) G->AddQuestRegion("Q_OrchardFarmRaidGood", "OrchardFarm");
        else G->AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm");
        GuardKilled = BanditKilled = Met = false; CrateCount = 0; Stolen = NoneStolen = false; Timer = 0;
        Done = Wrong = Spawned = Succeeded = false; Failed = 0; Waves = 0; T1 = T2 = Cut = false;
        Teams[0].EnemyTeam = &Teams[1]; Teams[1].EnemyTeam = &Teams[0]; Teams[0].Carrier = 0; Teams[1].Carrier = 0;
        Reasons[0] = "PROBLEM"; Reasons[1] = "HERO_KILLED"; Reasons[2] = "CRATES_STOLEN"; Reasons[3] = "TEAM_KILLED";
        if (G->IsQuestActive("Q_OrchardFarmRaidEvil")) {
            HeroTeam = 1; Code = "TEXT_QST_051_";
            G->SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside");
            G->MiniMap("OrchardFarm", "GreatwoodLake", false); G->MiniMap("OrchardFarm", "GreatwoodEntrance", true);
        } else if (G->IsQuestActive("Q_OrchardFarmRaidGood")) {
            HeroTeam = 0; Code = "TEXT_QST_052_";
            G->SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "", "Greatwood");
            G->MiniMap("OrchardFarm", "GreatwoodEntrance", false); G->MiniMap("OrchardFarm", "GreatwoodLake", true);
        }
    }
};
int main()
{
    Game g; Script s; std::memset(&s, 1, sizeof(s)); s.G = &g; s.HeroTeam = 7;
    g_active = "Q_OrchardFarmRaidGood"; s.Init();
    if (std::strcmp(g_log, "Q_OrchardFarmRaidGood;OrchardFarm;Q_OrchardFarmRaidGood;TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01;Greatwood;OrchardFarm;GreatwoodEntrance;0;OrchardFarm;GreatwoodLake;1;") != 0
        || s.HeroTeam != 0 || s.Teams[0].EnemyTeam != &s.Teams[1] || s.Teams[1].EnemyTeam != &s.Teams[0] || s.CrateCount != 0 || s.Met) {
        std::printf("00dcc140_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0; g_active = "Q_OrchardFarmRaidEvil"; s.Init();
    if (std::strcmp(g_log, "Q_OrchardFarmRaidEvil;OrchardFarm;Q_OrchardFarmRaidEvil;TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01;HeroGuildComplexInside;OrchardFarm;GreatwoodLake;0;OrchardFarm;GreatwoodEntrance;1;") != 0
        || s.HeroTeam != 1 || std::strcmp(s.Code, "TEXT_QST_051_") != 0) {
        std::printf("00dcc140_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0; g_active = "none"; s.HeroTeam = 5; s.Init();
    if (std::strcmp(g_log, "Q_OrchardFarmRaidEvil;OrchardFarm;") != 0 || s.HeroTeam != 5) { std::printf("00dcc140_TEST FAIL\n"); return 1; }
    std::printf("00dcc140_TEST PASS\n");
    return 0;
}
