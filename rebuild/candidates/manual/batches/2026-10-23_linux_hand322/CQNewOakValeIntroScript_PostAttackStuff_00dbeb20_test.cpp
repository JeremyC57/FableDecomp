#include <cstdio>
#include <cstring>
// behaviour test: CQ_NewOakValeIntroScript::PostAttackStuff -- wait for the start marker, post-attack setup, wait to reach
// the dad trigger, cut-scene with the hero cast inside a paused movie sequence, then restore
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0;
static bool g_paused = false;
struct CGameScriptInterface {
    void NewScriptFrame() const { ++g_frames; }
    bool MarkerAlive() const { return g_frames >= 2; }
    bool NearDad() const { return g_frames >= 5; }
    void CacheMusicSet(long s) const { char b[16]; std::sprintf(b, "music%ld", s); L(b); }
    void EntityTeleportToThing() const { L("teleport"); }
    void SetVillageLimbo(bool on) const { L(on ? "limbo" : "nolimbo"); }
    void DisplayMoneyBag(bool) const { L("bag"); }
    void TakeObjectFromHero(const char* o) const { L(o); }
    void CameraResetToViewBehindHero(float) { L("camreset"); }
    void CameraDefault() { L("camdefault"); }
    void FadeScreenIn() { L("fadein"); }
    void PauseAllNonScriptedEntities(bool p) { g_paused = p; }
    void FixMovieSequenceCamera(bool) const {}
    void SetTimeAsStopped(bool stopped, long& token) const { if (!stopped) token = 0; }
    void DeactivateQuest(const char* q) const { L(q); }
    void ResetToDefaultTheme(float) const { L("theme"); }
    void StopOverrideMusic(bool) const { L("stopmusic"); }
};
static void AddLogbookStoryEntry(long e) { if (e == 0x14) L("log"); }
static void RunCutsceneMacro(const char* n) { if (g_paused) L(n); }
struct CQ_NewOakValeIntroScript {
    CGameScriptInterface* Game; long TimeStoppedToken; bool HesDeadJimStarted;
    bool IsActiveThreadTerminating() { return false; }
    void PostAttackStuff()
    {
        while (!Game->MarkerAlive()) Game->NewScriptFrame();
        Game->CacheMusicSet(0x2d);
        Game->EntityTeleportToThing();
        Game->SetVillageLimbo(true);
        Game->DisplayMoneyBag(false);
        Game->TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE");
        AddLogbookStoryEntry(0x14);
        Game->NewScriptFrame();
        Game->CameraResetToViewBehindHero(0.0f); Game->CameraDefault(); Game->CacheMusicSet(0x39); Game->FadeScreenIn();
        while (!Game->NearDad()) Game->NewScriptFrame();
        HesDeadJimStarted = true;
        Game->PauseAllNonScriptedEntities(true); Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_OAKVALEINTRO_HESDEADJIM");
        Game->FixMovieSequenceCamera(false); Game->PauseAllNonScriptedEntities(false);
        Game->SetVillageLimbo(false);
        Game->SetTimeAsStopped(false, TimeStoppedToken);
        Game->DeactivateQuest("Q__OakValeIntro_PostAttack");
        Game->ResetToDefaultTheme(0.0f);
        Game->StopOverrideMusic(false);
    }
};
int main()
{
    CGameScriptInterface gsi; CQ_NewOakValeIntroScript s; s.Game = &gsi; s.TimeStoppedToken = 5; s.HesDeadJimStarted = false;
    s.PostAttackStuff();
    const char* want = "music45;teleport;limbo;bag;OBJECT_TEDDY_BEAR_UNGIVEABLE;log;camreset;camdefault;music57;fadein;CS_OAKVALEINTRO_HESDEADJIM;nolimbo;Q__OakValeIntro_PostAttack;theme;stopmusic;";
    if (std::strcmp(g_log, want) != 0 || !s.HesDeadJimStarted || s.TimeStoppedToken != 0 || g_paused) { std::printf("00dbeb20_TEST FAIL\n"); return 1; }
    std::printf("00dbeb20_TEST PASS\n");
    return 0;
}
