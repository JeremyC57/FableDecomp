#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeCaveScript::Init -- 11 quest regions, state reset, master-data flags, six boasts
static int g_regions = 0, g_boasts = 0, g_sumIds = 0, g_withAmount = 0;
struct CGameScriptInterface {
    void AddQuestRegion(const char* q, const char*) const { if (!std::strcmp(q, "Q_HobbeCave")) ++g_regions; }
    void AddBoast(const char*, long id, long cost, long reward, bool hasAmount, const char*, long) const { ++g_boasts; g_sumIds += id; if (hasAmount) g_withAmount += id; if (cost != id * 10 || reward != id * 100) g_boasts = -100; }
};
struct CScriptDef { long Cost[6], Reward[6]; long Timer; };
static CScriptDef g_def;
struct CData { bool Done; long State; bool Active; };
struct CQ_HobbeCaveScript {
    CGameScriptInterface* Game; CData* PMasterData; bool f[9]; long c58, c5c; bool f60; long Timer; bool g[5]; long c70, c74;
    void Init()
    {
        const char* r[11] = { "GrannysHouse", "Greatwood", "GreatwoodCaves", "DemonDoor_GreatwoodCaves", "HobbeCaveEntranceTunnel", "HobbeCaveSideRoom", "HobbeCaveLarder", "HobbeCaveNymphChamber", "HobbeCaveMainChamber", "HobbeCaveNymphFocal", "DemonDoor_GreatwoodGrannysHouse" };
        for (int i = 0; i < 11; ++i) Game->AddQuestRegion("Q_HobbeCave", r[i]);
        for (int i = 0; i < 9; ++i) f[i] = false;
        c58 = 0; c5c = 0; f60 = false; Timer = g_def.Timer;
        for (int i = 0; i < 5; ++i) g[i] = false;
        c70 = 0; c74 = 0;
        PMasterData->Done = false; PMasterData->State = 1; PMasterData->Active = true;
        const long ids[6] = { 1, 3, 6, 0x15, 0x16, 0x17 };
        for (int i = 0; i < 6; ++i) Game->AddBoast("desc", ids[i], g_def.Cost[i], g_def.Reward[i], ids[i] == 0x16, "", 0);
    }
};
int main()
{
    const long ids[6] = { 1, 3, 6, 0x15, 0x16, 0x17 };
    for (int i = 0; i < 6; ++i) { g_def.Cost[i] = ids[i] * 10; g_def.Reward[i] = ids[i] * 100; }
    g_def.Timer = 1234;
    CGameScriptInterface gsi; CData d; d.Done = true; d.State = 0; d.Active = false;
    CQ_HobbeCaveScript s; std::memset(&s, 0xff, sizeof(s)); s.Game = &gsi; s.PMasterData = &d;
    s.Init();
    bool ok = g_regions == 11 && g_boasts == 6 && g_sumIds == 1 + 3 + 6 + 0x15 + 0x16 + 0x17 && g_withAmount == 0x16
        && s.Timer == 1234 && !s.f[8] && !s.g[4] && s.c74 == 0 && !d.Done && d.State == 1 && d.Active;
    if (!ok) { std::printf("00d8d640_TEST FAIL\n"); return 1; }
    std::printf("00d8d640_TEST PASS\n");
    return 0;
}