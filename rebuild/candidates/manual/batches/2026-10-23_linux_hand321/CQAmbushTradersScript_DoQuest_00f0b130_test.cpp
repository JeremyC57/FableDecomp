#include <cstdio>
#include <cstring>
#include <string>
// behaviour test: CQ_AmbushTradersScript::DoQuest -- waits for the contact intro, then for the hero to enter Bowerstone Jail
// from Bowerstone Posh (other regions / entrances keep it waiting), then activates phase 1, spawns three threads, runs the
// intro and both phases and finally waits for Bowerstone Posh when the quest finished cleanly
static int g_frames = 0, g_endAt = 1000, g_objectives = 0, g_activated = 0, g_spawned = 0, g_bandits = 0, g_intro = 0, g_start = 0, g_phases = 0, g_poshWaits = 0;
static std::string g_log;
struct CScriptThing { std::string data; std::string GetDataString() const { return data; } };
struct CQ_AmbushTradersScript;
struct CGameScriptInterface {
    CScriptThing hero;
    CQ_AmbushTradersScript* script;
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char* r) const { if (!std::strcmp(r, "BowerstonePosh")) { ++g_poshWaits; return g_poshWaits >= 3; } return false; }
    bool MsgOnRegionLoaded(std::string& region) const
    {
        // frame 2: the slums; frame 4: the jail by the wrong door; frame 6: the jail from Bowerstone Posh
        if (g_frames == 2) { region = "BowerstoneSlums"; return true; }
        if (g_frames == 4 || g_frames == 6) { region = "BowerstoneJail"; return true; }
        return false;
    }
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetNearestWithDefName(const CScriptThing&, const char* def) const
    { CScriptThing t; t.data = std::strcmp(def, "REGION_ENTRANCE_POINT") ? "" : (g_frames == 6 ? "FromBowerstonePosh" : "FromBowerstoneSouth"); return t; }
    void ActivateQuest(const char* q) const { if (!std::strcmp(q, "Q_AmbushTraders_ActivatePhase1")) ++g_activated; }
    std::string GetActiveQuestName() const { return "Q_AmbushTraders"; }
    void SetQuestCardObjective(const std::string& q, const char* text, const char* region, const char* level) const
    { if (q == "Q_AmbushTraders" && !std::strcmp(level, "BowerstoneSlums")) { ++g_objectives; g_log += text; g_log += region; } }
    void KickOffQuestStartScreen(const std::string& q, bool a, bool b) const { if (q == "Q_AmbushTraders" && a && !b) ++g_start; }
};
struct CQ_AmbushTradersScript {
    CGameScriptInterface* Game;
    bool QuestFailed, QuestFinished, BanditContactIntroComplete, BanditContactKilledBeforeQuestEnd;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void Spawn(const char* name) { ++g_spawned; g_log += name; }
    void GetBandits() { ++g_bandits; }
    void Intro() { ++g_intro; }
    void PhaseOne() { ++g_phases; }
    void PhaseTwo() { ++g_phases; QuestFinished = true; }
    void DoQuest();
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == 1) script->BanditContactIntroComplete = true; }
void CQ_AmbushTradersScript::DoQuest()
{
    std::string region;
    while (!BanditContactIntroComplete) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_02", "BowerstoneJail", "BowerstoneSlums");
    CScriptThing entrance;
    for (;;) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->MsgOnRegionLoaded(region)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        entrance = Game->GetNearestWithDefName(Game->GetHero(), "REGION_ENTRANCE_POINT");
        if (region == "BowerstoneJail" && entrance.GetDataString() == "FromBowerstonePosh") break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating()) return;
    if (IsActiveThreadTerminating()) return;
    Game->ActivateQuest("Q_AmbushTraders_ActivatePhase1");
    Spawn("WatchForRegionLeaving");
    Spawn("WatchForBanditsDeath");
    GetBandits();
    Spawn("IntroPart2Checker");
    Intro();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_03", "DesertedFarm", "BowerstoneSlums");
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    PhaseOne();
    PhaseTwo();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    if (QuestFailed) {
        if (IsActiveThreadTerminating()) return;
    } else if (QuestFinished && !BanditContactKilledBeforeQuestEnd) {
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsRegionLoaded("BowerstonePosh")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
    }
}
int main()
{
    CGameScriptInterface game; CQ_AmbushTradersScript s; s.Game = &game; game.script = &s;
    s.QuestFailed = s.QuestFinished = s.BanditContactIntroComplete = s.BanditContactKilledBeforeQuestEnd = false;
    s.DoQuest();
    bool ok = g_objectives == 2 && g_activated == 1 && g_spawned == 3 && g_bandits == 1 && g_intro == 1 && g_start == 1 && g_phases == 2
        && g_poshWaits == 3 && g_frames == 9
        && g_log == "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_02BowerstoneJailWatchForRegionLeavingWatchForBanditsDeathIntroPart2CheckerTEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_03DesertedFarm";
    g_frames = 0; g_endAt = 5; g_activated = 0; s.BanditContactIntroComplete = false;
    s.DoQuest();                                       // terminated while looking for the Posh entrance
    ok = ok && g_activated == 0 && g_frames == 5;
    if (!ok) { std::printf("00f0b130_TEST FAIL\n"); return 1; }
    std::printf("00f0b130_TEST PASS\n");
    return 0;
}
