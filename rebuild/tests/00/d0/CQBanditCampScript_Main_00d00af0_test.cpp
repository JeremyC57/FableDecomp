#include <cstdio>
#include <cstring>
// behaviour test: CQ_BanditCampScript::Main -- bindings, first-run quest card + raid activation, four threads, the gate
// wait with autosave, and taking the pass / key only when the hero carries them
static int g_frames = 0, g_endAt = 1000, g_openAt = 3, g_bound = 0, g_spawned = 0, g_posted = 0, g_card = 0, g_raid = 0, g_saves = 0, g_takenPass = 0, g_takenKey = 0;
static bool g_hasPass = true, g_hasKey = false;
struct CScriptThing { int id; };
struct CQ_BanditCampScript;
static CQ_BanditCampScript* g_script = 0;
struct CGameScriptInterface {
    CScriptThing hero;
    void NewScriptFrame() const;
    void PostAddScriptedEntities() const { ++g_posted; }
    CScriptThing& GetHero() { return hero; }
    void TakeObjectFromHero(const char* o) const { if (!std::strcmp(o, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")) ++g_takenPass; if (!std::strcmp(o, "OBJECT_BANDIT_CAMP_HOSTAGE_KEY")) ++g_takenKey; }
    bool IsObjectInThingsPossession(const char* o, const CScriptThing&) const
    { return (!std::strcmp(o, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS") && g_hasPass) || (!std::strcmp(o, "OBJECT_BANDIT_CAMP_HOSTAGE_KEY") && g_hasKey); }
    void ActivateQuest(const char* q) const { if (!std::strcmp(q, "Q_OakValeBanditRaid")) ++g_raid; }
    void SetQuestCardObjective(const char* q, const char*, const char*, const char*) const { if (!std::strcmp(q, "Q_BanditCamp")) ++g_card; }
    void AutoSaveCheckPoint() const { ++g_saves; }
};
struct CQ_BanditCampScript {
    CGameScriptInterface* GSI; bool Gate1Open, ActivatedBanditRaid;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void AddEntityScriptBinding(const char*) { ++g_bound; }
    void PostAddScriptedEntities() { ++g_posted; }
    void SpawnFunction(const char*) { ++g_spawned; }
    void Main();
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames >= g_openAt) g_script->Gate1Open = true; }
void CQ_BanditCampScript::Main()
{
    AddEntityScriptBinding("Gate1GuardOuter");
    AddEntityScriptBinding("Gate1GuardInner");
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    if (!ActivatedBanditRaid) {
        if (IsActiveThreadTerminating()) return;
        GSI->SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_01", "", "");
        GSI->ActivateQuest("Q_OakValeBanditRaid");
        ActivatedBanditRaid = true;
    }
    SpawnFunction("WatchForTermination"); SpawnFunction("WatchForCostume"); SpawnFunction("CheckAnyBanditsKilled"); SpawnFunction("WatchForEndOfScript");
    if (!Gate1Open) {
        if (IsActiveThreadTerminating()) return;
        while (!Gate1Open) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        GSI->AutoSaveCheckPoint();
    }
    if (GSI->IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", GSI->GetHero())) {
        if (IsActiveThreadTerminating()) return;
        GSI->TakeObjectFromHero("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS");
    }
    if (GSI->IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", GSI->GetHero())) {
        if (IsActiveThreadTerminating()) return;
        GSI->TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY");
    }
}
int main()
{
    CGameScriptInterface game; CQ_BanditCampScript s; s.GSI = &game; s.Gate1Open = false; s.ActivatedBanditRaid = false; g_script = &s;
    s.Main();
    bool ok = g_bound == 2 && g_posted == 2 && g_card == 1 && g_raid == 1 && s.ActivatedBanditRaid && g_spawned == 4 && g_frames == 3 && g_saves == 1 && g_takenPass == 1 && g_takenKey == 0;
    g_hasKey = true; g_hasPass = false;
    s.Main();                                           // second run: raid already active, gate already open
    ok = ok && g_card == 1 && g_raid == 1 && g_saves == 1 && g_takenPass == 1 && g_takenKey == 1 && g_spawned == 8;
    if (!ok) { std::printf("00d00af0_TEST FAIL\n"); return 1; }
    std::printf("00d00af0_TEST PASS\n");
    return 0;
}