// Behaviour test for NScript::CQ_TraderEscortScript::Main (0x00E00DB0): the quest main binds the fourteen Darkwood escort
// entity scripts (flags 1, EndTrader 0) in retail's order, posts them, sets the objective (quest name, objective text,
// "Darkwood1", "Greatwood"), spawns WatchForSurprisingBalverines then WatchForMissionRules, then runs one script frame at a
// time until the quest start screen was shown; the first frame in which the hero pickpockets adds the PICKPOCKETTRADERS boast
// (type 10, the all-traders-alive cost/reward, quest name, 0) and ends the loop. Terminate checks stop it. A log model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptDef { public: long Cost, Reward; };
static CScriptDef g_def = { 70, 300 };
static CScriptDef* pGlobalScriptDef = &g_def;
struct CQ_TraderEscortScript;
class CGame { public:
    CQ_TraderEscortScript* Owner;
    int Frames, PickpocketAt, ScreenAt, TerminateAt;
    void PostAddScriptedEntities() { Log("gpost"); }
    const char* GetActiveQuestName() { return "Q_TraderEscort"; }
    void SetQuestCardObjective(const char* q, const char* o, const char* r, const char* t) { Log("obj"); Log(q); Log(o); Log(r); Log(t); }
    void NewScriptFrame();
    bool HeroHasExpression(const char* e) { return std::strcmp(e, "EXPRESSION_PICKPOCKET") == 0 && Frames == PickpocketAt; }
    void AddBoast(const char* t, long type, long cost, long reward, bool b, const char* q, long z)
    {
        Log("boast"); Log(t); Log(q);
        if (type != 10 || cost != 70 || reward != 300 || !b || z != 0) Log("BADARGS");
    }
};
struct CQ_TraderEscortScript {
    CGame* Game;
    bool QuestStartScreened;
    bool Terminating;
    void Bind(const char* n, unsigned long flags) { Log("bind"); Log(n); Log(flags == 1 ? "1" : "0"); }
    void PostAddScriptedEntities() { Log("post"); }
    void Spawn(const char* n) { Log("spawn"); Log(n); }
    bool IsActiveThreadTerminating() { return Terminating; }
    void Main()
    {
        Bind("DarkwoodTrader", 1); Bind("FleshEatingBalverine", 1); Bind("InfectedBalverine", 1); Bind("TraderComment", 1);
        Bind("DarkwoodAssassinSpawn", 1); Bind("DarkwoodBalverineTrader", 1); Bind("RockTrollTrigger", 1);
        Bind("DarkwoodRockTroll", 1); Bind("MagicBarrier", 1); Bind("TE_BanditLeader", 1); Bind("BanditCronies", 1);
        Bind("DW5_Balv", 1); Bind("DW5_Bandit", 1); Bind("EndTrader", 0);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01", "Darkwood1", "Greatwood");
        Spawn("WatchForSurprisingBalverines");
        Spawn("WatchForMissionRules");
        while (!QuestStartScreened) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Game->HeroHasExpression("EXPRESSION_PICKPOCKET")) {
                if (IsActiveThreadTerminating()) return;
                Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS", 10, pGlobalScriptDef->Cost, pGlobalScriptDef->Reward, true, Game->GetActiveQuestName(), 0);
                break;
            }
        }
        if (IsActiveThreadTerminating()) return;
    }
};
void CGame::NewScriptFrame()
{
    ++Frames; Log("frame");
    if (Frames == ScreenAt) Owner->QuestStartScreened = true;
    if (Frames == TerminateAt) Owner->Terminating = true;
}
static const char* kPrefix =
    "bind;DarkwoodTrader;1;bind;FleshEatingBalverine;1;bind;InfectedBalverine;1;bind;TraderComment;1;"
    "bind;DarkwoodAssassinSpawn;1;bind;DarkwoodBalverineTrader;1;bind;RockTrollTrigger;1;bind;DarkwoodRockTroll;1;"
    "bind;MagicBarrier;1;bind;TE_BanditLeader;1;bind;BanditCronies;1;bind;DW5_Balv;1;bind;DW5_Bandit;1;bind;EndTrader;0;"
    "post;gpost;obj;Q_TraderEscort;TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_01;Darkwood1;Greatwood;"
    "spawn;WatchForSurprisingBalverines;spawn;WatchForMissionRules;";
static bool Run(bool screened, int pick, int screen, int term, const char* tail)
{
    CGame g; CQ_TraderEscortScript s;
    g.Owner = &s; g.Frames = 0; g.PickpocketAt = pick; g.ScreenAt = screen; g.TerminateAt = term;
    s.Game = &g; s.QuestStartScreened = screened; s.Terminating = false;
    g_log[0] = 0;
    s.Main();
    char expected[4096];
    std::strcpy(expected, kPrefix); std::strcat(expected, tail);
    if (std::strcmp(g_log, expected) != 0) { std::printf("%s\n", g_log); return false; }
    return true;
}
int main()
{
    bool ok = Run(true, 1, 0, 0, "")
        && Run(false, 3, 0, 0, "frame;frame;frame;boast;TEXT_QST_BOAST_DESCRIPTION_PICKPOCKETTRADERS;Q_TraderEscort;")
        && Run(false, 9, 2, 0, "frame;frame;")
        && Run(false, 9, 0, 2, "frame;frame;");
    if (!ok) { std::printf("00e00db0_TEST FAIL\n"); return 1; }
    std::printf("00e00db0_TEST PASS\n");
    return 0;
}