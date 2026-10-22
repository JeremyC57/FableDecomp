// Behaviour test for CQ_BanditCampBossBattleScript::Init @ 0x00D03610: the six Bandit Camp regions under the boss-battle
// quest name, then every state flag cleared and the king's health / angry-bandit count zeroed.
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Log;
    void AddQuestRegion(const char* q, const char* r) { Log.push_back(std::string(q) + "/" + r); }
};
struct Script {
    Game* G;
    bool Flags[27];
    long KingHealth, AngryBanditNeeded;
    void Init()
    {
        static const char* regions[6] = { "BanditCampPathEntrance", "BanditCampEntrance", "BanditCampPath1",
                                          "BanditCampCentre", "BanditCampBoss", "DemonDoor_BanditCampPath" };
        for (int i = 0; i < 6; ++i)
            G->AddQuestRegion("Q_BanditCampBossBattle", regions[i]);
        for (int i = 0; i < 27; ++i)
            Flags[i] = false;
        KingHealth = 0;
        AngryBanditNeeded = 0;
    }
};
int main()
{
    Game g;
    Script s; s.G = &g; s.KingHealth = 50; s.AngryBanditNeeded = 4;
    for (int i = 0; i < 27; ++i) s.Flags[i] = true;
    s.Init();
    bool ok = g.Log.size() == 6 && g.Log[0] == "Q_BanditCampBossBattle/BanditCampPathEntrance" &&
              g.Log[5] == "Q_BanditCampBossBattle/DemonDoor_BanditCampPath" && s.KingHealth == 0 && s.AngryBanditNeeded == 0;
    for (int i = 0; i < 27; ++i) ok = ok && !s.Flags[i];
    if (!ok) { std::printf("00d03610_TEST FAIL\n"); return 1; }
    std::printf("00d03610_TEST PASS\n");
    return 0;
}
