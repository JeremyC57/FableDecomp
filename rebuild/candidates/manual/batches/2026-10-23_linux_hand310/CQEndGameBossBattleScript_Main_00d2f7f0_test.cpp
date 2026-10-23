// Behaviour test for CQ_EndGameBossBattleScript::Main (0x00D2F7F0).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface {
    void PostAddScriptedEntities() { Log("gpost"); }
    const char* GetActiveQuestName() { return "Q_EndGameBossBattle"; }
    void GiveHeroQuestCardDirectly(const char* card, const char* quest, bool b) { Log(card); Log(quest); if (b) Log("badargs"); }
    void ActivateQuest(const char* q) { Log("activate"); Log(q); }
};
struct CQ_EndGameBossBattleScript {
    CGameScriptInterface* Game;
    int flags[4]; int n;
    void AddEntityScriptBinding(const char* name, int f) { Log("bind"); Log(name); flags[n++] = f; }
    void PostAddScriptedEntities() { Log("post"); }
    void JackOfBladesBossBattle() { Log("battle"); }
    void Main()
    {
        AddEntityScriptBinding("SwordVoid", 0);
        AddEntityScriptBinding("EndGameSister", 0);
        AddEntityScriptBinding("EndGameJackOfBladesBoss", 1);
        AddEntityScriptBinding("TheRealGuildmaster", 1);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        Game->GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_JACK_FIGHT", Game->GetActiveQuestName(), false);
        Game->ActivateQuest("Q_EndGameBossBattleScenery");
        JackOfBladesBossBattle();
    }
};
int main()
{
    g_log[0] = 0;
    CGameScriptInterface game;
    CQ_EndGameBossBattleScript s; s.Game = &game; s.n = 0;
    s.Main();
    bool ok = std::strcmp(g_log, "bind;SwordVoid;bind;EndGameSister;bind;EndGameJackOfBladesBoss;bind;TheRealGuildmaster;post;gpost;"
        "OBJECT_QUEST_CARD_JACK_FIGHT;Q_EndGameBossBattle;activate;Q_EndGameBossBattleScenery;battle;") == 0
        && s.flags[0] == 0 && s.flags[1] == 0 && s.flags[2] == 1 && s.flags[3] == 1;
    if (!ok) { std::printf("00d2f7f0_TEST FAIL\n"); return 1; }
    std::printf("00d2f7f0_TEST PASS\n");
    return 0;
}
