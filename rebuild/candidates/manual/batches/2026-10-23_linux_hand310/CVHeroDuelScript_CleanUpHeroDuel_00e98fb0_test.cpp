// Behaviour test for CV_HeroDuelScript::CleanUpHeroDuel (0x00E98FB0): the call sequence through a logging game stub.
#include <cstdio>
#include <cstring>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    const char* name;
    bool alive;
    CScriptThing() : name(""), alive(false) {}
    bool IsAlive() const { return alive; }
};
struct CGameScriptInterface {
    bool evilDone, goodDone, helmetExists;
    void NewScriptFrame() { Log("frame"); }
    bool IsLevelLoaded(const char* level) { Log(level); return true; }
    void SetTeleportingAsActive(bool on) { Log(on ? "teleport-on" : "teleport-off"); }
    bool IsQuestCompleted(const char* q) { return std::strcmp(q, "Q_HangingTreeEvil") == 0 ? evilDone : goodDone; }
    void AddQuestCard(const char* card, const char*, bool, bool) { Log(card); }
    CScriptThing GetThingWithScriptName(const char* name) { CScriptThing t; t.name = name; t.alive = helmetExists; return t; }
    void OpenDoor(const CScriptThing& t) { Log("open"); Log(t.name); }
    const char* GetActiveQuestName() { return "V_HeroDuel"; }
    void DeactivateQuestLater(const char* q, unsigned long) { Log("deactivate"); Log(q); }
};
struct CMasterScriptData { long _unk00; long Clock; };
struct CV_HeroDuelScript {
    CGameScriptInterface* Game;
    CMasterScriptData* PMasterData;
    bool IsActiveThreadTerminating() { return false; }
    void CleanUpHeroDuel()
    {
        while (!Game->IsLevelLoaded("GibbetWoods")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        Game->SetTeleportingAsActive(true);
        if (PMasterData->Clock >= 1100) {
            if (IsActiveThreadTerminating()) return;
            if (!Game->IsQuestCompleted("Q_HangingTreeEvil") && !Game->IsQuestCompleted("Q_HangingTreeGood")) {
                if (IsActiveThreadTerminating()) return;
                Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false);
                Game->AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false);
            }
        }
        CScriptThing helmet;
        helmet = Game->GetThingWithScriptName("DD_ThunderHelmet");
        if (helmet.IsAlive())
            Game->OpenDoor(helmet);
        Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    }
};
static bool Run(long clock, bool evil, bool good, bool helmet, const char* expect)
{
    g_log[0] = 0;
    CGameScriptInterface game;
    game.evilDone = evil;
    game.goodDone = good;
    game.helmetExists = helmet;
    CMasterScriptData data;
    data.Clock = clock;
    CV_HeroDuelScript s;
    s.Game = &game;
    s.PMasterData = &data;
    s.CleanUpHeroDuel();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    bool ok = Run(1100, false, false, true, "GibbetWoods;teleport-on;OBJECT_QUEST_CARD_HANGING_TREE_EVIL;OBJECT_QUEST_CARD_HANGING_TREE_GOOD;open;DD_ThunderHelmet;deactivate;V_HeroDuel;")
        && Run(1099, false, false, false, "GibbetWoods;teleport-on;deactivate;V_HeroDuel;")
        && Run(2000, true, false, false, "GibbetWoods;teleport-on;deactivate;V_HeroDuel;")
        && Run(2000, false, true, true, "GibbetWoods;teleport-on;open;DD_ThunderHelmet;deactivate;V_HeroDuel;");
    if (!ok) { std::printf("00e98fb0_TEST FAIL\n"); return 1; }
    std::printf("00e98fb0_TEST PASS\n");
    return 0;
}
