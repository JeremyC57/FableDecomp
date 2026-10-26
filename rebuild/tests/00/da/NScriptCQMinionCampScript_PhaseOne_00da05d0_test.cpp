#include <cstdio>
#include <cstring>
// behaviour test: CQ_MinionCampScript::PhaseOne -- wait for the intro (stage 2, start the first-wave control), wait
// for Briar to reach the altar (stage 3, start the spawn-point control), then hold the minion cap at 3 at the 48 and
// 24 second chant marks until the portal is sealed; stage 4 unless the mission failed
static int g_frames = 0, g_endAt = 1000, g_firstWave = 0, g_spawnPoints = 0, g_capResets = 0;
static int g_introAt = 2, g_altarAt = 4, g_sealAt = 9; static bool g_fail = false;
struct CTimer { long Value; long GetTime() const { return Value; } };
struct CQ_MinionCampScript;
struct CGameScriptInterface { CQ_MinionCampScript* Owner; void NewScriptFrame() const; };
struct CQ_MinionCampScript {
    CGameScriptInterface* Game; CTimer ChantTimer; long CurrentStage, MaxNumberOfMinions;
    bool IntroDone, PortalSealed, MissionFailed, BriarRoseAtAltar;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void SpawnFunction(const char* n) { if (!std::strcmp(n, "RunFirstWaveControl")) ++g_firstWave; if (!std::strcmp(n, "RunSpawnPointControl")) ++g_spawnPoints; }
    void PhaseOne();
};
void CGameScriptInterface::NewScriptFrame() const
{
    ++g_frames;
    if (g_frames == g_introAt) Owner->IntroDone = true;
    if (g_frames == g_altarAt) Owner->BriarRoseAtAltar = true;
    Owner->ChantTimer.Value = g_frames == 6 ? 0x30 : g_frames == 7 ? 0x18 : 0;
    if (g_frames == 6 || g_frames == 7) Owner->MaxNumberOfMinions = 9;
    if (g_frames == g_sealAt) { if (g_fail) Owner->MissionFailed = true; else Owner->PortalSealed = true; }
}
void CQ_MinionCampScript::PhaseOne()
{
    while (!IntroDone && !PortalSealed && !MissionFailed) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CurrentStage = 2;
    SpawnFunction("RunFirstWaveControl");
    while (!BriarRoseAtAltar && !PortalSealed && !MissionFailed) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (BriarRoseAtAltar) { CurrentStage = 3; SpawnFunction("RunSpawnPointControl"); }
    while (!PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (ChantTimer.GetTime() == 0x30) { MaxNumberOfMinions = 3; ++g_capResets; }
        if (ChantTimer.GetTime() == 0x18) { MaxNumberOfMinions = 3; ++g_capResets; }
    }
    if (IsActiveThreadTerminating()) return;
    if (MissionFailed) return;
    CurrentStage = 4;
}
static void Reset(CQ_MinionCampScript& s)
{ g_frames = 0; s.CurrentStage = 1; s.MaxNumberOfMinions = 3; s.IntroDone = s.PortalSealed = s.MissionFailed = s.BriarRoseAtAltar = false; s.ChantTimer.Value = 0; }
int main()
{
    CGameScriptInterface game; CQ_MinionCampScript s; game.Owner = &s; s.Game = &game;
    Reset(s); s.PhaseOne();
    bool ok = s.CurrentStage == 4 && g_firstWave == 1 && g_spawnPoints == 1 && g_capResets == 2 && s.MaxNumberOfMinions == 3 && g_frames == 9;
    Reset(s); g_fail = true; s.PhaseOne();              // Briar dies: stays in stage 3
    ok = ok && s.CurrentStage == 3 && g_firstWave == 2 && g_spawnPoints == 2;
    Reset(s); g_fail = false; g_altarAt = 100; g_sealAt = 3; s.PhaseOne();   // sealed before Briar reached the altar
    ok = ok && s.CurrentStage == 4 && g_spawnPoints == 2;
    if (!ok) { std::printf("00da05d0_TEST FAIL\n"); return 1; }
    std::printf("00da05d0_TEST PASS\n");
    return 0;
}