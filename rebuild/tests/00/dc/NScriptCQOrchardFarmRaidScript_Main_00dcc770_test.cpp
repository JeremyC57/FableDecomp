#include <cstdio>
#include <cstring>
// behaviour test: CQ_OrchardFarmRaidScript::Main -- nine entity bindings, activate the barricade quest when inactive,
// wait for OrchardFarm, copy both crate drop points (counted-pointer assignment), spawn the hero team's game-rules
// thread plus the cutscene and deactivation watchers
static int g_frames, g_endAt, g_loadedAt, g_bound, g_activated, g_freed;
static bool g_barricadeActive;
static char g_spawned[256];
class CCPPointerInfo { public: long RefCount; void* Data; };
class CScriptThing { public:
    void* Data; CCPPointerInfo* Info;
    CScriptThing() : Data(0), Info(0) {}
    CScriptThing(const CScriptThing& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CScriptThing() { Release(); }
    void Release() { if (Info) { --Info->RefCount; if (Info->RefCount == 0) ++g_freed; } }
    CScriptThing& operator=(const CScriptThing& o)
    {
        CCPPointerInfo* info = o.Info; void* data = o.Data;
        if (Info != info) { Release(); Data = data; Info = info; if (info) ++info->RefCount; }
        return *this;
    }
};
static CCPPointerInfo g_banditInfo, g_guardInfo;
static int g_banditObj, g_guardObj;
class CGameScriptInterface { public:
    void NewScriptFrame() const { ++g_frames; }
    bool IsRegionLoaded(const char* n) const { return !std::strcmp(n, "OrchardFarm") && g_frames >= g_loadedAt; }
    void PostAddScriptedEntities() const {}
    CScriptThing GetThingWithScriptName(const char* n) const
    {
        CScriptThing t;
        if (!std::strcmp(n, "BanditTeamCrateDrop")) { t.Data = &g_banditObj; t.Info = &g_banditInfo; ++g_banditInfo.RefCount; }
        if (!std::strcmp(n, "GuardTeamCrateDrop")) { t.Data = &g_guardObj; t.Info = &g_guardInfo; ++g_guardInfo.RefCount; }
        return t;
    }
    void ActivateQuest(const char* q) const { if (!std::strcmp(q, "Q_OrchardFarm_Barricade")) ++g_activated; }
    bool IsQuestActive(const char* q) const { return !std::strcmp(q, "Q_OrchardFarm_Barricade") && g_barricadeActive; }
};
struct CCrateTeamManager { CScriptThing CrateDropPos; };
struct CQ_OrchardFarmRaidScript {
    CGameScriptInterface* Game; long HeroTeam; CCrateTeamManager Teams[2];
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() {}
    void SpawnFunction(const char* n) { std::strcat(g_spawned, n); std::strcat(g_spawned, ";"); }
    void Main();
};
void CQ_OrchardFarmRaidScript::Main()
{
    AddEntityScriptBinding("GuardTeamSpawn"); AddEntityScriptBinding("BanditTeamSpawn"); AddEntityScriptBinding("Artefact");
    AddEntityScriptBinding("OrchardFarmWhisper"); AddEntityScriptBinding("GuardTeamMember"); AddEntityScriptBinding("BanditTeamMember");
    AddEntityScriptBinding("FarmRearEntrance"); AddEntityScriptBinding("M_WhisperFarmRaidIntro"); AddEntityScriptBinding("MK_OFI_GWLL_WHIS2");
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    if (!Game->IsQuestActive("Q_OrchardFarm_Barricade"))
        Game->ActivateQuest("Q_OrchardFarm_Barricade");
    while (!Game->IsRegionLoaded("OrchardFarm")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Teams[1].CrateDropPos = Game->GetThingWithScriptName("BanditTeamCrateDrop");
    Teams[0].CrateDropPos = Game->GetThingWithScriptName("GuardTeamCrateDrop");
    if (HeroTeam == 1) SpawnFunction("ProcessGameRulesEvil");
    else SpawnFunction("ProcessGameRulesGood");
    SpawnFunction("DoCutsceneIfRequired");
    SpawnFunction("WatchForExternalScriptDeactivation");
}
int main()
{
    bool ok = true;
    CGameScriptInterface game;
    {
        CQ_OrchardFarmRaidScript s; s.Game = &game; s.HeroTeam = 1;
        g_frames = 0; g_endAt = 100; g_loadedAt = 3; g_barricadeActive = false; g_spawned[0] = 0;
        s.Main();
        ok = ok && g_bound == 9 && g_activated == 1 && s.Teams[1].CrateDropPos.Data == &g_banditObj && s.Teams[0].CrateDropPos.Data == &g_guardObj
                && g_banditInfo.RefCount == 1 && g_guardInfo.RefCount == 1
                && !std::strcmp(g_spawned, "ProcessGameRulesEvil;DoCutsceneIfRequired;WatchForExternalScriptDeactivation;");
        // second run with the barricade active and the good team: reassigning the same pointers keeps the counts
        s.HeroTeam = 2; g_frames = 0; g_barricadeActive = true; g_spawned[0] = 0;
        s.Main();
        ok = ok && g_activated == 1 && g_banditInfo.RefCount == 1 && g_guardInfo.RefCount == 1
                && !std::strcmp(g_spawned, "ProcessGameRulesGood;DoCutsceneIfRequired;WatchForExternalScriptDeactivation;");
        // the thread ends before the farm loads: nothing assigned or spawned
        CQ_OrchardFarmRaidScript t; t.Game = &game; t.HeroTeam = 1;
        g_frames = 0; g_endAt = 2; g_loadedAt = 50; g_spawned[0] = 0;
        t.Main();
        ok = ok && t.Teams[0].CrateDropPos.Info == 0 && g_spawned[0] == 0;
    }
    ok = ok && g_banditInfo.RefCount == 0 && g_guardInfo.RefCount == 0 && g_freed == 2;
    if (!ok) { std::printf("00dcc770_TEST FAIL\n"); return 1; }
    std::printf("00dcc770_TEST PASS\n");
    return 0;
}