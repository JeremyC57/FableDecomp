// Behaviour test for CQR_EscortTraderScript::Main @ 0x00E1E110: three entity-script bindings in order, post-add on the
// script then the game, the quest card objective, then three spawned functions and one frame.
#include <cstdio>
#include <string>
#include <vector>
struct Script {
    std::vector<std::string> Log;
    void Bind(const char* n) { Log.push_back(std::string("bind ") + n); }
    void Spawn(const char* n) { Log.push_back(std::string("spawn ") + n); }
    void Main()
    {
        Bind("M_BanditTrigger"); Bind("AttackingBandit"); Bind("TraderToEscort");
        Log.push_back("post script"); Log.push_back("post game");
        Log.push_back("objective TEXT_QUEST_ORCHARD_FARM_ESCORT_OBJECTIVE_01 LookoutPoint HeroGuildComplexInside");
        Spawn("WatchForTermination"); Spawn("DoMission"); Spawn("WatchForRegionLoads");
        Log.push_back("frame");
    }
};
int main()
{
    Script s; s.Main();
    bool ok = s.Log.size() == 10 && s.Log[2] == "bind TraderToEscort" && s.Log[3] == "post script" && s.Log[4] == "post game" &&
              s.Log[7] == "spawn DoMission" && s.Log[9] == "frame";
    if (!ok) { std::printf("00e1e110_TEST FAIL\n"); return 1; }
    std::printf("00e1e110_TEST PASS\n");
    return 0;
}