// Behaviour test for CQ_BanditCampScript::Init @ 0x00D006D0: six quest regions, the quest flags cleared, the master
// data's kill-many / kill-none boast flags reset (false / true), then the three boasts (ids 3, 24, 27).
#include <cstdio>
#include <string>
#include <vector>
struct Game {
    std::vector<std::string> Log;
    void AddQuestRegion(const char* q, const char* r) { Log.push_back(std::string(q) + "/" + r); }
    void AddBoast(int id, bool a, int b) { char buf[32]; std::sprintf(buf, "boast %d %d %d", id, a ? 1 : 0, b); Log.push_back(buf); }
};
struct Data { bool KillMany, KillNone; };
struct Script {
    Game* G; Data* D;
    bool Gate1Open, MissionSucceeded, MissionFailed, BoastBegun;
    void Init()
    {
        G->AddQuestRegion("Q_BanditCamp", "BanditCampPathEntrance");
        G->AddQuestRegion("Q_BanditCamp", "BanditCampEntrance");
        G->AddQuestRegion("Q_BanditCamp", "BanditCampPath1");
        G->AddQuestRegion("Q_BanditCamp", "BanditCampCentre");
        G->AddQuestRegion("Q_BanditCamp", "BanditCampBoss");
        G->AddQuestRegion("Q_BanditCamp", "DemonDoor_BanditCampPath");
        Gate1Open = MissionSucceeded = MissionFailed = BoastBegun = false;
        D->KillMany = false;
        D->KillNone = true;
        G->AddBoast(3, false, 0);
        G->AddBoast(24, true, 1);
        G->AddBoast(27, true, 1);
    }
};
int main()
{
    Game g; Data d = { true, false };
    Script s; s.G = &g; s.D = &d; s.Gate1Open = s.MissionSucceeded = s.MissionFailed = s.BoastBegun = true;
    s.Init();
    bool ok = g.Log.size() == 9 && g.Log[5] == "Q_BanditCamp/DemonDoor_BanditCampPath" && g.Log[6] == "boast 3 0 0" &&
              g.Log[8] == "boast 27 1 1" && !d.KillMany && d.KillNone && !s.Gate1Open && !s.BoastBegun;
    if (!ok) { std::printf("00d006d0_TEST FAIL\n"); return 1; }
    std::printf("00d006d0_TEST PASS\n");
    return 0;
}
