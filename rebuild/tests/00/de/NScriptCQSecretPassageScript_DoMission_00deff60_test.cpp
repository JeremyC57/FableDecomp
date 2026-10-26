#include <cstdio>
#include <cstring>
// Behaviour test for CQ_SecretPassageScript::DoMission (0x00DEFF60): waits for KrakenChamber, shows the start
// screen once, spawns the two threads, disables teleporting, runs PhaseOne and EndMission unless the mission failed.
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGSI {
    int LoadAfter; int Frames;
    void NewScriptFrame() { ++Frames; Log("Frame"); }
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "KrakenChamber") == 0 && Frames >= LoadAfter; }
    void KickOffQuestStartScreen(const char* q, bool a, bool b) { char t[64]; std::sprintf(t, "Kick:%s:%d:%d", q, a, b); Log(t); }
    void SetTeleportingAsActive(bool a) { char t[32]; std::sprintf(t, "Teleport:%d", a); Log(t); }
};
struct CQ_SecretPassageScript {
    CGSI* GSI; bool MissionFailed; bool QuestStartScreenShown;
    bool IsActiveThreadTerminating() { return false; }
    void SpawnFunction(const char* name) { char t[64]; std::sprintf(t, "Spawn:%s", name); Log(t); }
    void PhaseOne() { Log("PhaseOne"); }
    void EndMission() { Log("EndMission"); }
    void DoMission()
    {
        while (!GSI->IsRegionLoaded("KrakenChamber")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!QuestStartScreenShown) {
            if (IsActiveThreadTerminating())
                return;
            GSI->KickOffQuestStartScreen("Q_SecretPassage", true, false);
            QuestStartScreenShown = true;
        }
        SpawnFunction("WatchForLeaving");
        SpawnFunction("BossBlockingPrisonPassage");
        GSI->SetTeleportingAsActive(false);
        PhaseOne();
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (!MissionFailed)
            EndMission();
    }
};
int main()
{
    CGSI gsi; gsi.LoadAfter = 2; gsi.Frames = 0;
    CQ_SecretPassageScript s; s.GSI = &gsi; s.MissionFailed = false; s.QuestStartScreenShown = false;
    g_log[0] = 0; s.DoMission();
    bool ok = std::strcmp(g_log, "Frame;Frame;Kick:Q_SecretPassage:1:0;Spawn:WatchForLeaving;Spawn:BossBlockingPrisonPassage;Teleport:0;PhaseOne;Frame;EndMission;") == 0 && s.QuestStartScreenShown;
    gsi.Frames = 5; s.MissionFailed = true; g_log[0] = 0; s.DoMission();
    ok = ok && std::strcmp(g_log, "Spawn:WatchForLeaving;Spawn:BossBlockingPrisonPassage;Teleport:0;PhaseOne;Frame;") == 0;
    if (!ok) { std::printf("00deff60_TEST FAIL\n"); return 1; }
    std::printf("00deff60_TEST PASS\n");
    return 0;
}