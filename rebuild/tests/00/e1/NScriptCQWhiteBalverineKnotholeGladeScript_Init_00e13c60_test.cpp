// Behaviour test for CQ_WhiteBalverineKnotholeGladeScript::Init (0x00E13C60): three quest regions, two boasts priced
// from the script def, and every quest-state member reset.
#include <cstdio>
#include <cstring>
class CCharString { public:
    char Text[48];
    CCharString(const char* text) { std::strncpy(Text, text, 47); Text[47] = 0; }
};
class CGameScriptInterface { public:
    char log[512];
    void AddQuestRegion(const CCharString& quest, const CCharString& region) {
        std::strcat(log, quest.Text); std::strcat(log, ">"); std::strcat(log, region.Text); std::strcat(log, ";");
    }
    void AddBoast(const CCharString& d, long id, long cost, long reward, bool flag, const CCharString& extra, long v) {
        char b[160];
        std::sprintf(b, "%s#%ld#%ld#%ld#%d#%s#%ld;", d.Text, id, cost, reward, flag ? 1 : 0, extra.Text, v);
        std::strcat(log, b);
    }
};
class CScriptDef { public: long NakedCost, NakedReward, NoDamageCost, NoDamageReward; };
static CScriptDef g_def = { 10, 20, 30, 40 };
static CScriptDef* g_pScriptDef = &g_def;
struct CScript {
    CGameScriptInterface* Game;
    bool MissionFailed, MissionSucceeded, WifeCutsceneStart, WifeCutsceneStop, GreenWife;
    long BalverineState; bool QuestStartScreenShown;
    void Init();
};
void CScript::Init()
{
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "KnotholeGlade");
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "Witchwood4");
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "DemonDoor_KnotholeGlade");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_pScriptDef->NakedCost, g_pScriptDef->NakedReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_pScriptDef->NoDamageCost, g_pScriptDef->NoDamageReward, false, "", 0);
    BalverineState = 0;
    MissionSucceeded = false;
    MissionFailed = false;
    WifeCutsceneStart = false;
    WifeCutsceneStop = false;
    GreenWife = false;
    QuestStartScreenShown = false;
}
int main()
{
    CGameScriptInterface g; g.log[0] = 0;
    CScript s; s.Game = &g; s.MissionFailed = s.MissionSucceeded = s.WifeCutsceneStart = s.WifeCutsceneStop = true;
    s.GreenWife = s.QuestStartScreenShown = true; s.BalverineState = 9;
    s.Init();
    bool ok = std::strcmp(g.log, "Q_WhiteBalverineKnotholeGlade>KnotholeGlade;Q_WhiteBalverineKnotholeGlade>Witchwood4;"
        "Q_WhiteBalverineKnotholeGlade>DemonDoor_KnotholeGlade;TEXT_QST_BOAST_DESCRIPTION_NAKED#1#10#20#0##0;"
        "TEXT_QST_BOAST_DESCRIPTION_NODAMAGE#3#30#40#0##0;") == 0
        && s.BalverineState == 0 && !s.MissionFailed && !s.MissionSucceeded && !s.WifeCutsceneStart
        && !s.WifeCutsceneStop && !s.GreenWife && !s.QuestStartScreenShown;
    if (!ok) { std::printf("00e13c60_TEST FAIL\n"); return 1; }
    std::printf("00e13c60_TEST PASS\n");
    return 0;
}