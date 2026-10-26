#include <cstdio>
#include <cstring>
// behaviour test: CQ_RansomVictimScript::WatchForTermination -- when the mission ends, put the chief's hut back under
// normal house rules, record the kidnappers as killed when all three died, complete or fail the quest (victim died),
// deactivate the three side quests, switch the Witchwood 2-4 generators back on, deactivate the quest and fade in
static int g_frames = 0, g_endAt = 1000, g_hutReset = 0, g_completed = 0, g_failed = 0, g_deactivated = 0, g_generators = 0;
static int g_later = 0, g_fadeIns = 0, g_endAtFrame = 3;
struct SMasterData { bool RansomVictimKidnappersKilled; };
struct CScriptThing { int id; };
struct CQ_RansomVictimScript;
struct CGameScriptInterface {
    CQ_RansomVictimScript* Owner;
    void NewScriptFrame() const;
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.id = !std::strcmp(n, "ChiefsHut") ? 4 : 0; return t; }
    void SetBuyableHouseAsScripted(const CScriptThing& t, bool s) const { if (t.id == 4 && !s) ++g_hutReset; }
    void OverrideAutomaticHouseLocking(const CScriptThing& t, bool s) const { if (t.id == 4 && !s) ++g_hutReset; }
    void SetQuestAsCompleted() const { ++g_completed; }
    void SetQuestAsFailed(const char* why) const { if (!std::strcmp(why, "TEXT_QST_B06_QUEST_FAILED_VICTIM_DIED")) ++g_failed; }
    void DeactivateQuest(const char* q, long d) const { if (!std::strncmp(q, "Q_RansomVictim", 14) && d == 0) ++g_deactivated; }
    void SetCreatureGeneratorsEnabled(const char* r, bool on) const { if (!std::strncmp(r, "Witchwood", 9) && on) ++g_generators; }
    void DeactivateQuestLater(long d) const { if (d == 0) ++g_later; }
    void FadeScreenIn() const { ++g_fadeIns; }
};
struct CQ_RansomVictimScript {
    CGameScriptInterface* Game; SMasterData* PMasterData; long KidnappersKilled; bool MissionSucceeded, MissionFailed, Win;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void WatchForTermination();
};
void CGameScriptInterface::NewScriptFrame() const { if (++g_frames == g_endAtFrame) { if (Owner->Win) Owner->MissionSucceeded = true; else Owner->MissionFailed = true; } }
void CQ_RansomVictimScript::WatchForTermination()
{
    while (!MissionSucceeded && !MissionFailed) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetBuyableHouseAsScripted(Game->GetThingWithScriptName("ChiefsHut"), false);
    Game->OverrideAutomaticHouseLocking(Game->GetThingWithScriptName("ChiefsHut"), false);
    if (KidnappersKilled == 3) PMasterData->RansomVictimKidnappersKilled = true;
    if (MissionSucceeded) Game->SetQuestAsCompleted();
    else Game->SetQuestAsFailed("TEXT_QST_B06_QUEST_FAILED_VICTIM_DIED");
    Game->DeactivateQuest("Q_RansomVictimNaturesRevenge", 0);
    Game->DeactivateQuest("Q_RansomVictimRevenge", 0);
    Game->DeactivateQuest("Q_RansomVictimStarted", 0);
    Game->SetCreatureGeneratorsEnabled("Witchwood2", true);
    Game->SetCreatureGeneratorsEnabled("Witchwood3", true);
    Game->SetCreatureGeneratorsEnabled("Witchwood4", true);
    Game->DeactivateQuestLater(0);
    Game->FadeScreenIn();
}
int main()
{
    CGameScriptInterface game; SMasterData md; md.RansomVictimKidnappersKilled = false;
    CQ_RansomVictimScript s; game.Owner = &s; s.Game = &game; s.PMasterData = &md;
    s.KidnappersKilled = 3; s.MissionSucceeded = false; s.MissionFailed = false; s.Win = true;
    s.WatchForTermination();
    bool ok = g_hutReset == 2 && md.RansomVictimKidnappersKilled && g_completed == 1 && g_failed == 0 && g_deactivated == 3
           && g_generators == 3 && g_later == 1 && g_fadeIns == 1 && g_frames == 3;
    g_frames = 0; md.RansomVictimKidnappersKilled = false; s.KidnappersKilled = 2; s.MissionSucceeded = false; s.Win = false;
    s.WatchForTermination();                            // the victim died; not all kidnappers killed
    ok = ok && !md.RansomVictimKidnappersKilled && g_failed == 1 && g_completed == 1 && g_deactivated == 6 && g_later == 2;
    if (!ok) { std::printf("00de14a0_TEST FAIL\n"); return 1; }
    std::printf("00de14a0_TEST PASS\n");
    return 0;
}