// Behaviour test for CV_RandomPopulationSimScript::ProcessAlbionPopulationSim (0x00EC2240).
#include <cstdio>
#include <cstring>
#include <string>
static char g_log[2048];
static void Log(const std::string& s) { std::strcat(g_log, s.c_str()); std::strcat(g_log, ";"); }
struct RegionDef { long MaxPopulation[3]; };
static const RegionDef* FindDef(const std::string& n)
{
    static const RegionDef oak = { { 1, 4, 2 } }, empty = { { 0, 0, 0 } }, lake = { { 0, 3, 0 } };
    if (n == "REGION_OAKVALE") return &oak;
    if (n == "REGION_LOOKOUTPOINT") return &empty;
    if (n == "REGION_GREATWOODLAKE") return &lake;
    return 0;
}
static std::string Upper(std::string s) { for (unsigned i = 0; i < s.size(); ++i) if (s[i] >= 'a' && s[i] <= 'z') s[i] = (char)(s[i] - 32); return s; }
struct CGameScriptInterface {
    int frame; const char* const* loads; bool bounty;
    bool MsgOnRegionLoaded(std::string& r) { if (loads[frame]) { r = loads[frame]; return true; } return false; }
    bool IsQuestActive(const std::string& q) { return q == "Q_BountyHunt" && bounty; }
    void NewScriptFrame() { ++frame; }
};
struct CV_RandomPopulationSimScript {
    CGameScriptInterface* Game; bool HighDetailSimulationActive; long PopulationSim[3]; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SetCurrentPopulationRegionFromRegionName(std::string& r) { Log("set:" + r); }
    void ProcessAlbionPopulationSim()
    {
        std::string region;
        while (!IsActiveThreadTerminating()) {
            if (Game->MsgOnRegionLoaded(region)) {
                if (IsActiveThreadTerminating()) return;
                bool bountyHuntRegion = false;
                if (Game->IsQuestActive("Q_BountyHunt")) {
                    if (IsActiveThreadTerminating()) return;
                    if (region == "GreatwoodEntrance" || region == "GreatwoodLake" || region == "Fisherman") {
                        if (IsActiveThreadTerminating()) return;
                        bountyHuntRegion = true;
                    }
                }
                HighDetailSimulationActive = false;
                const RegionDef* def = 0;
                std::string defName = std::string("REGION_") + Upper(region);
                if (!bountyHuntRegion && (def = FindDef(defName)) != 0) {
                    if (IsActiveThreadTerminating()) return;
                    SetCurrentPopulationRegionFromRegionName(region);
                    PopulationSim[0] = def->MaxPopulation[0];
                    PopulationSim[1] = def->MaxPopulation[1];
                    PopulationSim[2] = def->MaxPopulation[2];
                    if (PopulationSim[0] == 0 && PopulationSim[1] == 0 && PopulationSim[2] == 0) {
                        if (IsActiveThreadTerminating()) return;
                        HighDetailSimulationActive = false;
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        HighDetailSimulationActive = true;
                    }
                }
                Log(HighDetailSimulationActive ? "active" : "inactive");
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(bool bounty, const char* expect)
{
    static const char* const loads[] = { 0, "OakVale", 0, "GreatwoodLake", "LookoutPoint", "Nowhere", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.loads = loads; g.bounty = bounty;
    CV_RandomPopulationSimScript s; s.Game = &g; s.HighDetailSimulationActive = true; s.termAfter = 25;
    s.ProcessAlbionPopulationSim();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return true;
}
int main()
{
    bool ok = Run(false, "set:OakVale;active;set:GreatwoodLake;active;set:LookoutPoint;inactive;inactive;")
        && Run(true, "set:OakVale;active;inactive;set:LookoutPoint;inactive;inactive;");
    if (!ok) { std::printf("00ec2240_TEST FAIL\n"); return 1; }
    std::printf("00ec2240_TEST PASS\n");
    return 0;
}