#include <cstdio>
#include <stdlib.h>
// Behaviour test for CQ_MinionCampScript::StartMinionSpawnTimer (0x00D9F200): the timer gets a whole-second wait inside [min, max].
static long g_timer = -1, g_time = -1;
struct CGameScriptInterface { virtual void SetTimer(long timer, long time) const { g_timer = timer; g_time = time; } };
static CGameScriptInterface g_game; static CGameScriptInterface* g_pGameScriptInterface = &g_game;
struct CTimer { long Handle; };
class CScriptBase { public: void* __vftable; };
class CQ_MinionCampScript : public CScriptBase {
public:
    unsigned char _pad04[0x4c]; long NumberOfMinions; long MaxNumberOfMinions; float MinMinionSpawnPeriod; float MaxMinionSpawnPeriod;
    unsigned char _pad60[0x10]; CTimer MinionSpawnTimer;
    void StartMinionSpawnTimer()
    {
        float r = rand() * (1.0f / RAND_MAX);
        g_pGameScriptInterface->SetTimer(MinionSpawnTimer.Handle, (long)((MaxMinionSpawnPeriod - MinMinionSpawnPeriod) * r + MinMinionSpawnPeriod));
    }
};
int main()
{
    CQ_MinionCampScript c; c.MinionSpawnTimer.Handle = 9; c.MinMinionSpawnPeriod = 10.0f; c.MaxMinionSpawnPeriod = 20.0f;
    int ok = 1;
    for (int i = 0; i < 50; ++i) { c.StartMinionSpawnTimer(); if (g_timer != 9 || g_time < 10 || g_time > 20) ok = 0; }
    c.MinMinionSpawnPeriod = 4.0f; c.MaxMinionSpawnPeriod = 4.0f; c.StartMinionSpawnTimer(); if (g_time != 4) ok = 0;
    std::printf(ok ? "00d9f200_TEST PASS\n" : "00d9f200_TEST FAIL\n"); return ok ? 0 : 1;
}