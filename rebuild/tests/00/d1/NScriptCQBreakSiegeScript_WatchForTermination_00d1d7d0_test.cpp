#include <cstdio>
#include <cstring>
// behaviour test: CQ_BreakSiegeScript::WatchForTermination -- limbo + siege theme until the quest ends (re-applied on a
// Knothole Glade reload), then lift the limbo, remove the blockage, open the gate, wait for the cutscene, fail with the
// reason text or complete, restore, and deactivate the quest once the region unloads
static int g_frames = 0, g_endAt = 1000, g_limboOn = 0, g_limboOff = 0, g_themes = 0, g_removed = 0, g_opened = 0, g_failed = 0, g_completed = 0, g_deactivated = 0, g_unloadAt = 0;
static const char* g_reason = "";
struct CQ_BreakSiegeScript;
static CQ_BreakSiegeScript* g_s = 0;
class CScriptThing { public: const char* name; bool IsAlive() const { return std::strcmp(name, "Blockage") == 0; } };
class CGameScriptInterface { public:
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char*) const { return g_frames < g_unloadAt; }
    bool MsgOnRegionLoaded(const char*& region) const { if (g_frames == 2) { region = "KnotholeGlade"; return true; } return false; }
    CScriptThing GetThingWithScriptName(const char* n) const { CScriptThing t; t.name = n; return t; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!std::strcmp(t.name, "Blockage") && !a && b) ++g_removed; }
    void SetWeaponOutCrimeEnabled(bool) const {}
    void DeactivateQuestLater(const char*, unsigned long) const { ++g_deactivated; }
    void SetQuestAsCompleted(const char*, bool, bool, bool) const { ++g_completed; }
    void SetQuestAsFailed(const char*, bool, const char* reason, bool) const { ++g_failed; g_reason = reason; }
    void FadeScreenIn() {}
    void Pause(float) const {}
    void OpenDoor(const CScriptThing& d) const { if (!std::strcmp(d.name, "KG_Gate")) ++g_opened; }
    void SetVillageLimbo(const CScriptThing& v, bool on) const { if (!std::strcmp(v.name, "VILLAGE_KNOTHOLEGLADE")) { if (on) ++g_limboOn; else ++g_limboOff; } }
    const char* GetActiveQuestName() const { return "Q_BreakSiege"; }
    void TransitionToTheme(const char*, float) const { ++g_themes; }
    void ResetToDefaultTheme(float) const {}
    void EnableDecals(bool) const {}
};
struct CData { bool BreakSiegeCompleted; };
struct CQ_BreakSiegeScript {
    CGameScriptInterface* Game; CData* Data; bool MissionSucceeded, MissionFailed, EndCutsceneStarts, EndCutsceneDone; long ReasonForFailure;
    bool IsActiveThreadTerminating() { return g_frames >= g_endAt; }
    void WatchForTermination();
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == 4) g_s->MissionFailed = g_s->ReasonForFailure != 0, g_s->MissionSucceeded = !g_s->MissionFailed; if (g_s->EndCutsceneStarts && g_frames >= 6) g_s->EndCutsceneDone = true; }
void CQ_BreakSiegeScript::WatchForTermination()
{
    CScriptThing village = Game->GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE");
    const char* region = "";
    Game->SetVillageLimbo(village, true);
    Game->TransitionToTheme("theme", 0.5f);
    Game->EnableDecals(false);
    while (!MissionFailed && !MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Game->MsgOnRegionLoaded(region) && std::strcmp(region, "KnotholeGlade") == 0) {
            if (IsActiveThreadTerminating()) return;
            Game->SetVillageLimbo(village, true);
            Game->TransitionToTheme("theme", 0.5f);
            Game->EnableDecals(false);
        }
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetVillageLimbo(village, false);
    if (Game->GetThingWithScriptName("Blockage").IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Game->RemoveThing(Game->GetThingWithScriptName("Blockage"), false, true);
    }
    CScriptThing gate = Game->GetThingWithScriptName("KG_Gate");
    Game->OpenDoor(gate);
    EndCutsceneStarts = true;
    while (!EndCutsceneDone) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->SetWeaponOutCrimeEnabled(true);
    if (MissionFailed) {
        if (IsActiveThreadTerminating()) return;
        const char* reason;
        if (ReasonForFailure == 1) reason = "TEXT_QST_018_QUEST_FAILED_PRISONER_DIED";
        else if (ReasonForFailure == 2) reason = "TEXT_QST_018_QUEST_FAILED_CHIEF_DIED";
        else if (ReasonForFailure == 3) reason = "TEXT_QST_018_QUEST_FAILED_PRISONER_ESCAPED";
        else reason = "There is no reason";
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, reason, true);
    } else {
        if (IsActiveThreadTerminating()) return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Data->BreakSiegeCompleted = true;
    }
    Game->ResetToDefaultTheme(0.2f); Game->EnableDecals(true); Game->Pause(0.1f); Game->FadeScreenIn();
    while (Game->IsRegionLoaded("KnotholeGlade") != false) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
int main()
{
    CGameScriptInterface game; CData data; data.BreakSiegeCompleted = false;
    CQ_BreakSiegeScript s; g_s = &s; s.Game = &game; s.Data = &data;
    s.MissionSucceeded = s.MissionFailed = s.EndCutsceneStarts = s.EndCutsceneDone = false; s.ReasonForFailure = 0; g_unloadAt = 8;
    s.WatchForTermination();                            // success path
    bool ok = g_limboOn == 2 && g_themes == 2 && g_limboOff == 1 && g_removed == 1 && g_opened == 1 && g_completed == 1 && data.BreakSiegeCompleted && g_deactivated == 1 && g_frames == 8;
    g_frames = 0; s.MissionSucceeded = s.MissionFailed = s.EndCutsceneStarts = s.EndCutsceneDone = false; s.ReasonForFailure = 2;
    s.WatchForTermination();                            // failure path with the chief-died reason
    ok = ok && g_failed == 1 && std::strcmp(g_reason, "TEXT_QST_018_QUEST_FAILED_CHIEF_DIED") == 0 && g_completed == 1 && g_deactivated == 2;
    if (!ok) { std::printf("00d1d7d0_TEST FAIL\n"); return 1; }
    std::printf("00d1d7d0_TEST PASS\n");
    return 0;
}