#include <cstdio>
#include <cstring>
// behaviour test: CQ_SummoningTheShipScript::DoMission -- success path (all summoners dead, rose alive) and failure path
static char g_log[4096];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script;
static Script* g_s;
static int g_frame = 0;
static bool g_fail = false;
struct Script {
    long SummonersAlive, CurrentAttackWave; bool LighthouseStarted, SummonerAttacksStarted, MissionFailed; long ConversationIndex;
    bool IsActiveThreadTerminating() { return false; }
    void Frame();
    void Cut(const char* n) { L(n); }
    void Run()
    {
        L("objective1");
        while (g_frame < 1) { Frame(); }
        L("prepare"); Cut("CS_SUMSHIP_PREINTRO"); L("start");
        while (!LighthouseStarted) Frame();
        L("objective2"); Cut("CS_SUMSHIP_INTRO"); SummonerAttacksStarted = true;
        while (CurrentAttackWave <= 1 && !MissionFailed) Frame();
        if (MissionFailed) { Cut("CS_SUMSHIP_OUTRO_FAILED"); L("failed"); } else Cut("CS_SUMSHIP_WAVE2");
        while (SummonersAlive != 0 && !MissionFailed) Frame();
        bool roseAlive = true;
        if (roseAlive) { ConversationIndex = 9; L("rose"); }
        if (SummonersAlive == 0) { Cut("CS_SUMSHIP_OUTRO"); L("movie"); L("complete"); } else { Cut("CS_SUMSHIP_OUTRO_FAILED"); L("failed"); }
        L("restore"); L("teleport"); L("deactivate");
    }
};
void Script::Frame()
{
    ++g_frame;
    if (g_frame == 2) LighthouseStarted = true;
    if (g_frame == 3) { if (g_fail) MissionFailed = true; else CurrentAttackWave = 2; }
    if (g_frame == 5) SummonersAlive = 0;
}
int main()
{
    Script s; s.SummonersAlive = 4; s.CurrentAttackWave = 0; s.LighthouseStarted = s.SummonerAttacksStarted = s.MissionFailed = false; s.ConversationIndex = -1; g_s = &s;
    s.Run();
    if (std::strcmp(g_log, "objective1;prepare;CS_SUMSHIP_PREINTRO;start;objective2;CS_SUMSHIP_INTRO;CS_SUMSHIP_WAVE2;rose;CS_SUMSHIP_OUTRO;movie;complete;restore;teleport;deactivate;") != 0 || !s.SummonerAttacksStarted || s.ConversationIndex != 9) {
        std::printf("00df36e0_TEST FAIL\n"); return 1;
    }
    g_log[0] = 0; g_frame = 0; g_fail = true; s.SummonersAlive = 4; s.CurrentAttackWave = 0; s.LighthouseStarted = s.MissionFailed = false;
    s.Run();
    if (std::strcmp(g_log, "objective1;prepare;CS_SUMSHIP_PREINTRO;start;objective2;CS_SUMSHIP_INTRO;CS_SUMSHIP_OUTRO_FAILED;failed;rose;CS_SUMSHIP_OUTRO_FAILED;failed;restore;teleport;deactivate;") != 0) {
        std::printf("00df36e0_TEST FAIL\n"); return 1;
    }
    std::printf("00df36e0_TEST PASS\n");
    return 0;
}