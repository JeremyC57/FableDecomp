#include <cstdio>
#include <cstring>
// behaviour test: CQ_NewOakValeIntroScript::DoMission -- first-run intro setup, wait for the intro flag, attack phases,
// then restore and complete/deactivate; a second run skips the setup block
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0;
struct CQ_NewOakValeIntroScript;
static CQ_NewOakValeIntroScript* g_s;
class CGameScriptInterface { public:
    void NewScriptFrame() const;
    bool IsRegionLoaded(const char*) const { return g_frames > 0; }
    void FadeScreenOutUntilNextCallToFadeScreenIn(float t, float) { L(t == 2.0f ? "fade2" : "fade05"); }
    void TurnCreatureInto(const char* d) const { L(d); }
    void CacheMusicSet(long s) const { if (s == 0x2e) L("cache"); }
    void ActivateQuest(const char* q) const { L(q); }
    void EntitySetAsKillable(bool k, bool) const { L(k ? "killable" : "unkillable"); }
    void SetTimeAsStopped(bool, long& token) const { token = 77; }
    void SetTimeOfDay(float h) const { if (h == 12.0f) L("noon"); }
    void SetHeroSleepingAsEnabled(bool on) const { L(on ? "sleep" : "nosleep"); }
    void DisplayMoneyBag(bool) const { L("bag"); }
    void OverrideAutomaticHouseLocking(bool) const { L("unlock"); }
    void OpenHouseDoors() const { L("doors"); }
    void KickOffQuestStartScreen(bool, bool) const { L("start"); }
    void OverrideMusic(long, bool, bool) const { L("music"); }
    void SetQuestAsCompleted() const { L("complete"); }
    void DeactivateQuestLater() const { L("deactivate"); }
};
struct CQ_NewOakValeIntroScript {
    CGameScriptInterface* Game; long TimeStoppedToken; bool IntroDone;
    bool IsActiveThreadTerminating() { return false; }
    void Spawn(const char* n) { L(n); }
    void AttackStuff() { L("attack"); }
    void PostAttackStuff() { L("post"); }
    void DoMission()
    {
        while (!Game->IsRegionLoaded("StartOakVale")) Game->NewScriptFrame();
        if (!IntroDone) {
            Game->FadeScreenOutUntilNextCallToFadeScreenIn(2.0f, 0.0f);
            Game->TurnCreatureInto("CREATURE_HERO_CHILD");
            Spawn("WatchBarrels"); Spawn("WatchForGotGold"); Spawn("ManageQuestCoreMarkers");
            Game->CacheMusicSet(0x2e);
            Game->ActivateQuest("Q_NewOakValeIntro_PreAttack");
            Game->NewScriptFrame();
            Game->EntitySetAsKillable(false, false);
            Game->SetTimeAsStopped(true, TimeStoppedToken);
            Game->SetTimeOfDay(12.0f);
            Game->SetHeroSleepingAsEnabled(false);
            Game->DisplayMoneyBag(true);
            Game->OverrideAutomaticHouseLocking(true);
            Game->OpenHouseDoors();
            Game->KickOffQuestStartScreen(false, true);
            Game->OverrideMusic(0x13, false, true);
        }
        while (!IntroDone) Game->NewScriptFrame();
        AttackStuff(); PostAttackStuff();
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
        Game->EntitySetAsKillable(true, false);
        Game->SetHeroSleepingAsEnabled(true);
        Game->SetQuestAsCompleted();
        Game->DeactivateQuestLater();
    }
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == 4) g_s->IntroDone = true; }
int main()
{
    CGameScriptInterface gsi; CQ_NewOakValeIntroScript s; g_s = &s; s.Game = &gsi; s.IntroDone = false; s.TimeStoppedToken = 0;
    s.DoMission();
    const char* want = "fade2;CREATURE_HERO_CHILD;WatchBarrels;WatchForGotGold;ManageQuestCoreMarkers;cache;Q_NewOakValeIntro_PreAttack;unkillable;noon;nosleep;bag;unlock;doors;start;music;attack;post;fade05;killable;sleep;complete;deactivate;";
    if (std::strcmp(g_log, want) != 0 || s.TimeStoppedToken != 77) { std::printf("00dbde40_TEST FAIL\n"); return 1; }
    g_log[0] = 0; s.DoMission();
    if (std::strcmp(g_log, "attack;post;fade05;killable;sleep;complete;deactivate;") != 0) { std::printf("00dbde40_TEST FAIL\n"); return 1; }
    std::printf("00dbde40_TEST PASS\n");
    return 0;
}