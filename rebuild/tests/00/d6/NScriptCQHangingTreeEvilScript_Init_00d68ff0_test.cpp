#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeEvilScript::Init -- register the four quest regions, reset the mission state, switch the four
// regions' creature generators off and register the four boasts (the last one: PROTECTBANDITS, id 32)
struct SScriptDef { long Cost[4]; long Reward[4]; };
static SScriptDef g_def;
static int g_regions = 0, g_generatorsOff = 0, g_boasts = 0, g_lastId = 0; static bool g_lastFlag = false; static long g_costSum = 0, g_rewardSum = 0;
static const char* g_regionNames[4] = { "BowerstoneJail", "DesertedFarm", "GibbetWoods", "ExecutionTree" };
class CGameScriptInterface { public:
    void AddQuestRegion(const char* q, const char* r) const { if (!std::strcmp(q, "Q_HangingTreeEvil") && !std::strcmp(r, g_regionNames[g_regions])) ++g_regions; }
    void SetCreatureGeneratorsEnabled(const char*, bool on) const { if (!on) ++g_generatorsOff; }
    void AddBoast(const char* text, long id, long cost, long reward, bool flag) const
    { ++g_boasts; g_lastId = id; g_lastFlag = flag; g_costSum += cost; g_rewardSum += reward; if (std::strncmp(text, "TEXT_QST_BOAST_DESCRIPTION_", 27)) g_boasts = -99; }
};
struct CQ_HangingTreeEvilScript {
    CGameScriptInterface* Game; bool MissionFailed, MissionSucceeded, SpawnHere, SwitchingRegions;
    void Init();
};
void CQ_HangingTreeEvilScript::Init()
{
    for (int i = 0; i < 4; ++i) Game->AddQuestRegion("Q_HangingTreeEvil", g_regionNames[i]);
    MissionFailed = false; MissionSucceeded = false; SwitchingRegions = false; SpawnHere = true;
    for (int i = 0; i < 4; ++i) Game->SetCreatureGeneratorsEnabled(g_regionNames[i], false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, g_def.Cost[0], g_def.Reward[0], false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, g_def.Cost[1], g_def.Reward[1], false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NOWEAPONS", 6, g_def.Cost[2], g_def.Reward[2], false);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_PROTECTBANDITS", 32, g_def.Cost[3], g_def.Reward[3], 1 != 0);
}
int main()
{
    for (int i = 0; i < 4; ++i) { g_def.Cost[i] = i + 1; g_def.Reward[i] = 10 * (i + 1); }
    CGameScriptInterface game; CQ_HangingTreeEvilScript s; s.Game = &game; s.MissionFailed = true; s.SpawnHere = false;
    s.Init();
    bool ok = g_regions == 4 && g_generatorsOff == 4 && g_boasts == 4 && g_lastId == 32 && g_lastFlag == (1 != 0)
           && g_costSum == 10 && g_rewardSum == 100 && !s.MissionFailed && s.SpawnHere && !s.SwitchingRegions;
    if (!ok) { std::printf("00d68ff0_TEST FAIL\n"); return 1; }
    std::printf("00d68ff0_TEST PASS\n");
    return 0;
}