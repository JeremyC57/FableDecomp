// Behaviour test for 0x00D2F640 NScript::CQ_EndGameFocalSitesScript::~CQ_EndGameFocalSitesScript (implicit): timers deregistered
// in reverse member order, then RegionDialogueNames[4..0], TeleportHSPs[4..0], then the CScriptBase base dtor.
#include <cstdio>
static int g_log[64]; static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
class CTimer { public: long TimerIndex; ~CTimer() { Log(1000 + (int)TimerIndex); } };        // DeregisterTimer(TimerIndex)
class CCharString { public: int Id; ~CCharString() { Log(Id); } };
class CScriptBase { public: virtual ~CScriptBase() { Log(9999); } };
class C3DVector { public: float X, Y, Z; };
struct CQ_EndGameFocalSitesScript : public CScriptBase {
    void* Game; void* MasterScriptData;
    long CurrentFocalSiteRegion; CCharString TeleportHSPs[5]; CCharString RegionDialogueNames[5]; C3DVector TeleportPosition;
    bool MissionFailed, JackLeaves, EndCutScenePlayed, QuestStartScreenShown; long counter; CTimer JackTimer; CTimer WorldTimer; bool JackReached;
};
int main()
{
    {
        CQ_EndGameFocalSitesScript s;
        for (int i = 0; i < 5; ++i) { s.TeleportHSPs[i].Id = 10 + i; s.RegionDialogueNames[i].Id = 20 + i; }
        s.JackTimer.TimerIndex = 7; s.WorldTimer.TimerIndex = 8;
    }
    static const int expect[] = { 1008, 1007, 24, 23, 22, 21, 20, 14, 13, 12, 11, 10, 9999 };
    if (g_n != 13) { std::printf("00d2f640_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 13; ++i) if (g_log[i] != expect[i]) { std::printf("00d2f640_TEST FAIL\n"); return 1; }
    std::printf("00d2f640_TEST PASS\n");
    return 0;
}