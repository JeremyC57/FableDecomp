#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionCampScript::DoMission -- wait for Darkwood_5 and the intro, show the quest start screen
// once, fade in and run phase one; then wait for failure or stage 5: a failure after stage 2 plays
// CS_MINIONCAMP_FAILED before failing the quest, success completes it; either way the Darkwood3 generators come back
// on and the quest is deactivated
static int g_frames = 0, g_endAt = 1000, g_startScreens = 0, g_fadeIns = 0, g_phases = 0, g_failed = 0, g_completed = 0;
static int g_generatorsOn = 0, g_deactivated = 0, g_cutscenes = 0, g_stage5At = 6; static bool g_failNow = false;
struct CQ_MinionCampScript;
struct CGameScriptInterface {
    CQ_MinionCampScript* Owner;
    void NewScriptFrame() const;
    bool IsLevelLoaded(const char* n) const { return !std::strcmp(n, "Darkwood_5") && g_frames >= 1; }
    void KickOffQuestStartScreen() const { ++g_startScreens; }
    void FadeScreenIn() const { ++g_fadeIns; }
    void SetQuestAsFailed(const char* why) const { if (!std::strcmp(why, "TEXT_QST_B08_QUEST_FAILED_BRIARDIED")) ++g_failed; }
    void SetQuestAsCompleted() const { ++g_completed; }
    void SetCreatureGeneratorsEnabled(const char* r, bool on) const { if (!std::strcmp(r, "Darkwood3") && on) ++g_generatorsOn; }
    void DeactivateQuestLater(long d) const { if (d == 0) ++g_deactivated; }
};
struct CQ_MinionCampScript {
    CGameScriptInterface* Game; long CurrentStage; bool IntroDone, QuestStartScreenShown, MissionFailed;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void PhaseOne() { ++g_phases; CurrentStage = 3; }
    void RunCutsceneMacro(const char* n) { if (!std::strcmp(n, "CS_MINIONCAMP_FAILED")) ++g_cutscenes; }
    void DoMission();
};
void CGameScriptInterface::NewScriptFrame() const
{
    if (++g_frames == 2) Owner->IntroDone = true;
    if (g_frames == g_stage5At) { if (g_failNow) Owner->MissionFailed = true; else Owner->CurrentStage = 5; }
}
void CQ_MinionCampScript::DoMission()
{
    while (!Game->IsLevelLoaded("Darkwood_5")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CurrentStage = 1;
    while (!IntroDone) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!QuestStartScreenShown) { Game->KickOffQuestStartScreen(); QuestStartScreenShown = true; }
    Game->FadeScreenIn();
    PhaseOne();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!MissionFailed && CurrentStage != 5) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (MissionFailed) {
        if (CurrentStage > 2) RunCutsceneMacro("CS_MINIONCAMP_FAILED");
        Game->SetQuestAsFailed("TEXT_QST_B08_QUEST_FAILED_BRIARDIED");
    } else {
        Game->SetQuestAsCompleted();
        Game->FadeScreenIn();
    }
    Game->SetCreatureGeneratorsEnabled("Darkwood3", true);
    Game->DeactivateQuestLater(0);
}
static void Reset(CQ_MinionCampScript& s) { g_frames = 0; s.CurrentStage = 0; s.IntroDone = false; s.MissionFailed = false; }
int main()
{
    CGameScriptInterface game; CQ_MinionCampScript s; game.Owner = &s; s.Game = &game; s.QuestStartScreenShown = false;
    Reset(s); s.DoMission();
    bool ok = g_startScreens == 1 && g_phases == 1 && g_completed == 1 && g_fadeIns == 2 && g_failed == 0 && g_generatorsOn == 1 && g_deactivated == 1;
    Reset(s); g_failNow = true; s.DoMission();          // Briar died in stage 3: cutscene, failed; start screen not repeated
    ok = ok && g_startScreens == 1 && g_cutscenes == 1 && g_failed == 1 && g_completed == 1 && g_generatorsOn == 2 && g_deactivated == 2;
    if (!ok) { std::printf("00da0230_TEST FAIL\n"); return 1; }
    std::printf("00da0230_TEST PASS\n");
    return 0;
}
