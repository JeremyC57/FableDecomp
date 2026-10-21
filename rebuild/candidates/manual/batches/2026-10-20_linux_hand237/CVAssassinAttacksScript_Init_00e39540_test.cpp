// Behaviour test for CV_AssassinAttacksScript::Init (0x00E39540): state reset, five kill counters, region records filled
// from the script def, and one WatchForQuestComplete thread spawned with an empty region name.
#include <cstdio>
#include <cstring>

struct CScriptDef { const char* Names[5]; long Limits[5]; };
static CScriptDef g_def = { { "A", "B", "C", "D", "E" }, { 1, 2, 3, 4, 5 } };
static CScriptDef* g_pScriptDef = &g_def;
struct RegionData { char name[8]; long count; long max; bool assassinActive; };
struct CSpawned { char Name[32]; void* Owner; };
static CSpawned* g_spawned; static char g_region[8] = "x";
struct CV_AssassinAttacksScript {
    long AssassinObjectCount; bool Activated; char RegionOfMostRecentKill[8];
    RegionData RegionInfo[5]; long RegionKillCounts[8]; int KillCountsSize;
    bool QuestComplete, RewardsGiven, GivenQuestCard;
    void AddSpawnedFunction(CSpawned* f, const char* region) { g_spawned = f; std::strcpy(g_region, region); }
    void Init()
    {
        AssassinObjectCount = 0;
        Activated = false;
        std::strcpy(RegionOfMostRecentKill, "");
        QuestComplete = false; RewardsGiven = false; GivenQuestCard = false;
        KillCountsSize = 5; for (int k = 0; k < 5; ++k) RegionKillCounts[k] = 0;
        for (long i = 0; i < 5; ++i) {
            RegionInfo[i].count = 0;
            RegionInfo[i].max = g_pScriptDef->Limits[i];
            std::strcpy(RegionInfo[i].name, g_pScriptDef->Names[i]);
            RegionInfo[i].assassinActive = false;
        }
        CSpawned* f = new CSpawned; std::strcpy(f->Name, "WatchForQuestComplete"); f->Owner = this;
        AddSpawnedFunction(f, "");
    }
};

int main()
{
    CV_AssassinAttacksScript s; std::memset(&s, 0x7f, sizeof(s));
    s.Init();
    bool ok = s.AssassinObjectCount == 0 && !s.Activated && s.RegionOfMostRecentKill[0] == 0 && !s.QuestComplete && !s.RewardsGiven && !s.GivenQuestCard;
    ok = ok && s.KillCountsSize == 5 && s.RegionKillCounts[4] == 0;
    for (int i = 0; i < 5; ++i) ok = ok && s.RegionInfo[i].count == 0 && s.RegionInfo[i].max == i + 1 && s.RegionInfo[i].name[0] == 'A' + i && !s.RegionInfo[i].assassinActive;
    ok = ok && g_spawned && std::strcmp(g_spawned->Name, "WatchForQuestComplete") == 0 && g_spawned->Owner == &s && g_region[0] == 0;
    delete g_spawned;
    if (!ok) { std::printf("00e39540_TEST FAIL\n"); return 1; }
    std::printf("00e39540_TEST PASS\n");
    return 0;
}
