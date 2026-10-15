#include <cstdio>
// Behaviour test for CQ_MinionCampScript::OnMinionKilled (0x00D9F1B0): the live count never goes below zero, and the spawn timer is restarted only when the camp is below its maximum and the old timer has run out.
static int g_timeLeft = 0; static int g_restarted = 0; static bool g_terminating = false;
struct CGameScriptInterface { virtual long GetTimer(long) const { return g_timeLeft; } };
static CGameScriptInterface g_game; static CGameScriptInterface* g_pGameScriptInterface = &g_game;
struct CTimer { long Handle; };
class CScriptBase { public: void* __vftable; bool IsActiveThreadTerminating() { return g_terminating; } };
class CQ_MinionCampScript : public CScriptBase {
public:
    unsigned char _pad04[0x4c]; long NumberOfMinions; long MaxNumberOfMinions; float MinMinionSpawnPeriod; float MaxMinionSpawnPeriod;
    unsigned char _pad60[0x10]; CTimer MinionSpawnTimer;
    void StartMinionSpawnTimer() { ++g_restarted; }
    void OnMinionKilled()
    {
        if (--NumberOfMinions < 0) { if (IsActiveThreadTerminating()) return; NumberOfMinions = 0; }
        if (NumberOfMinions < MaxNumberOfMinions) {
            if (g_pGameScriptInterface->GetTimer(MinionSpawnTimer.Handle) <= 0) { if (IsActiveThreadTerminating()) return; StartMinionSpawnTimer(); }
        }
    }
};
int main()
{
    CQ_MinionCampScript c; c.NumberOfMinions = 2; c.MaxNumberOfMinions = 3; c.MinionSpawnTimer.Handle = 7;
    g_timeLeft = 5; c.OnMinionKilled(); int ok = (c.NumberOfMinions == 1 && g_restarted == 0);   // timer still running
    g_timeLeft = 0; c.OnMinionKilled(); if (!(c.NumberOfMinions == 0 && g_restarted == 1)) ok = 0;  // expired: restart
    c.OnMinionKilled(); if (!(c.NumberOfMinions == 0 && g_restarted == 2)) ok = 0;                // clamped at zero
    c.NumberOfMinions = 3; c.OnMinionKilled(); if (g_restarted != 3) ok = 0;                      // 2 < 3 and expired
    c.NumberOfMinions = 4; c.OnMinionKilled(); if (g_restarted != 3) ok = 0;                      // 3 == max: no restart
    g_terminating = true; c.NumberOfMinions = 0; c.OnMinionKilled(); if (c.NumberOfMinions != -1) ok = 0;   // thread ending: bail before the clamp
    std::printf(ok ? "00d9f1b0_TEST PASS\n" : "00d9f1b0_TEST FAIL\n"); return ok ? 0 : 1;
}
