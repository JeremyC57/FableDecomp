#include <cstdio>
#include <cstring>
// behaviour test: CQ_HangingTreeGoodScript::RemoveParty -- re-collect the HTG_Guard things; with the exit camera,
// play the paused HTG_ExitCam fade-out; remove every live guard (popping the vector) and the live prisoner, then
// restore the camera / fade in (exit camera only) and mark the party as no longer set up
static int g_removed = 0, g_pauses = 0, g_cameraPoint = 0, g_default = 0, g_fadeIn = 0, g_fadeOut = 0; static float g_paused = 0;
struct CScriptThing { int id; bool alive; bool IsAlive() const { return alive; } };
static CScriptThing Make(int id, bool alive) { CScriptThing t; t.id = id; t.alive = alive; return t; }
struct CThings {
    CScriptThing Items[8]; unsigned int Count;
    CThings() : Count(0) { }
    void clear() { Count = 0; }
    void push_back(const CScriptThing& t) { Items[Count++] = t; }
    bool empty() const { return Count == 0; }
    const CScriptThing& back() const { return Items[Count - 1]; }
    void pop_back() { --Count; }
};
struct CGameScriptInterface {
    void GetAllThingsWithScriptName(const char* n, CThings& v) const
    { if (!std::strcmp(n, "HTG_Guard")) { v.push_back(Make(1, true)); v.push_back(Make(2, false)); v.push_back(Make(3, true)); } }
    CScriptThing GetThingWithScriptName(const char* n) const { return !std::strcmp(n, "HTG_Prisoner") ? Make(9, true) : Make(7, true); }
    void PauseAllNonScriptedEntities(bool p) const { g_pauses += p ? 1 : -1; }
    void CameraUseCameraPoint(const CScriptThing& t) const { if (t.id == 7) ++g_cameraPoint; }
    void Pause(float t) const { g_paused += t; }
    void FadeScreenOut() const { ++g_fadeOut; }
    void RemoveThing(const CScriptThing& t, bool a, bool b) const { if (!a && b && t.alive) ++g_removed; }
    void CameraDefault() const { ++g_default; }
    void FadeScreenIn() const { ++g_fadeIn; }
};
struct CQ_HangingTreeGoodScript {
    CGameScriptInterface* Game; CThings CurrentGuardVector; CScriptThing Prisoner; bool ExitCutsceneStart, PartySuccessfullySetUp;
    bool IsActiveThreadTerminating() const { return false; }
    void RemoveParty(bool useExitCamera);
};
void CQ_HangingTreeGoodScript::RemoveParty(bool useExitCamera)
{
    CurrentGuardVector.clear();
    Game->GetAllThingsWithScriptName("HTG_Guard", CurrentGuardVector);
    CScriptThing exitCam = Game->GetThingWithScriptName("HTG_ExitCam");
    if (useExitCamera) {
        if (IsActiveThreadTerminating()) return;
        Game->PauseAllNonScriptedEntities(true);
        Game->CameraUseCameraPoint(exitCam);
        ExitCutsceneStart = true;
        Game->Pause(3.0f);
        Game->FadeScreenOut();
        Game->Pause(1.0f);
        Game->PauseAllNonScriptedEntities(false);
    }
    while (!CurrentGuardVector.empty()) {
        if (CurrentGuardVector.back().IsAlive()) Game->RemoveThing(CurrentGuardVector.back(), false, true);
        CurrentGuardVector.pop_back();
    }
    Prisoner = Game->GetThingWithScriptName("HTG_Prisoner");
    if (Prisoner.IsAlive()) Game->RemoveThing(Prisoner, false, true);
    ExitCutsceneStart = false;
    if (useExitCamera) {
        if (IsActiveThreadTerminating()) return;
        Game->CameraDefault();
        Game->FadeScreenIn();
    }
    PartySuccessfullySetUp = false;
}
int main()
{
    CGameScriptInterface game; CQ_HangingTreeGoodScript s; s.Game = &game; s.PartySuccessfullySetUp = true;
    s.CurrentGuardVector.push_back(Make(5, true));      // stale entry: cleared first
    s.RemoveParty(true);
    bool ok = g_removed == 3 && g_cameraPoint == 1 && g_pauses == 0 && g_paused == 4.0f && g_fadeOut == 1 && g_default == 1
           && g_fadeIn == 1 && !s.ExitCutsceneStart && !s.PartySuccessfullySetUp && s.CurrentGuardVector.empty() && s.Prisoner.id == 9;
    g_removed = 0; s.PartySuccessfullySetUp = true;
    s.RemoveParty(false);                               // no camera work
    ok = ok && g_removed == 3 && g_cameraPoint == 1 && g_default == 1 && !s.PartySuccessfullySetUp;
    if (!ok) { std::printf("00d732f0_TEST FAIL\n"); return 1; }
    std::printf("00d732f0_TEST PASS\n");
    return 0;
}
