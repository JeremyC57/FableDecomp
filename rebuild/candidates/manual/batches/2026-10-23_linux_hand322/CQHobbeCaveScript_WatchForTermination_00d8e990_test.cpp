#include <cstdio>
#include <cstring>
// behaviour test: CQ_HobbeCaveScript::WatchForTermination -- generators off until the mission ends, back on, fail with the
// reason picked by FailReason (or complete), take the key if held, wait for the level to unload, deactivate
static char g_log[1024];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0, g_gensOn = 0, g_gensOff = 0, g_unloadAt = 5;
struct CQ_HobbeCaveScript;
static CQ_HobbeCaveScript* g_s = 0;
struct CGameScriptInterface {
    void NewScriptFrame() const;
    bool IsLevelLoaded(const char*) const { return g_frames < g_unloadAt; }
    void SetCreatureGeneratorsEnabled(const char*, bool on) const { if (on) ++g_gensOn; else ++g_gensOff; }
    void SetQuestAsFailed(const char*, bool, const char* reason, bool) const { L("failed"); L(reason); }
    void SetQuestAsCompleted(const char*, bool, bool, bool) const { L("completed"); }
    bool IsObjectInThingsPossession(const char*, const char*) const { return true; }
    void TakeObjectFromHero(const char* o) const { L(o); }
    void DeactivateQuestLater(const char*, unsigned long) const { L("deactivate"); }
};
struct CQ_HobbeCaveScript {
    CGameScriptInterface* Game; bool MissionSucceeded, MissionFailed; long FailReason; int failAt;
    bool IsActiveThreadTerminating() { return false; }
    void WatchForTermination()
    {
        for (int i = 0; i < 7; ++i) Game->SetCreatureGeneratorsEnabled("r", false);
        while (!MissionFailed && !MissionSucceeded) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        for (int i = 0; i < 7; ++i) Game->SetCreatureGeneratorsEnabled("r", true);
        if (MissionFailed) {
            if (FailReason == 0) Game->SetQuestAsFailed("Q", true, "GRANNY_DIED", true);
            else if (FailReason == 1) Game->SetQuestAsFailed("Q", true, "GRANDSON_DIED", true);
            else Game->SetQuestAsFailed("Q", true, "", true);
        } else Game->SetQuestAsCompleted("Q", true, true, false);
        bool hasKey = Game->IsObjectInThingsPossession("OBJECT_HEXAGON_DOOR_KEY", "hero");
        if (hasKey) Game->TakeObjectFromHero("OBJECT_HEXAGON_DOOR_KEY");
        while (Game->IsLevelLoaded("GrannysHouse") != false) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        Game->DeactivateQuestLater("Q", 0);
    }
};
void CGameScriptInterface::NewScriptFrame() const { ++g_frames; if (g_frames == g_s->failAt) g_s->MissionFailed = true; if (g_frames == 3 && g_s->failAt == 0) g_s->MissionSucceeded = true; }
int main()
{
    CGameScriptInterface gsi; CQ_HobbeCaveScript s; g_s = &s; s.Game = &gsi;
    s.MissionSucceeded = false; s.MissionFailed = false; s.FailReason = 1; s.failAt = 2;
    s.WatchForTermination();
    if (std::strcmp(g_log, "failed;GRANDSON_DIED;OBJECT_HEXAGON_DOOR_KEY;deactivate;") != 0 || g_gensOn != 7 || g_gensOff != 7 || g_frames != 5) { std::printf("00d8e990_TEST FAIL\n"); return 1; }
    g_log[0] = 0; g_frames = 0; s.MissionSucceeded = false; s.MissionFailed = false; s.failAt = 0;
    s.WatchForTermination();
    if (std::strcmp(g_log, "completed;OBJECT_HEXAGON_DOOR_KEY;deactivate;") != 0) { std::printf("00d8e990_TEST FAIL\n"); return 1; }
    std::printf("00d8e990_TEST PASS\n");
    return 0;
}
