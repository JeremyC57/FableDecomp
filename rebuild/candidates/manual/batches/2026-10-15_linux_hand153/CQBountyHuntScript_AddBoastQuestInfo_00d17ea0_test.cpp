// Behaviour test for 0x00d17ea0: AddBoastQuestInfo: start timer, add the timer info, flag and display.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct GSI { long timer, time; int adds; float scale; bool shown; long nextId;
  void SetTimer(long t, long v) { timer = t; time = v; }
  long AddQuestInfoTimer(long t, const char*, float s) { ++adds; scale = s; return nextId + t; }
  void DisplayQuestInfo(bool b) { shown = b; } };
static GSI g_gsi;
struct Script { GSI* gsi; long BoastTimerID; long BoastTime; bool Displayed;
  void Add() { g_gsi.SetTimer(BoastTime, 300); BoastTimerID = gsi->AddQuestInfoTimer(BoastTime, "HUD_CLOCK_ICON", 1.0f); Displayed = true; gsi->DisplayQuestInfo(true); } };
static int run() { g_gsi.adds = 0; g_gsi.nextId = 100; g_gsi.shown = false; Script s = {&g_gsi, 0, 4, false}; s.Add();
  CHECK(g_gsi.timer == 4 && g_gsi.time == 300 && s.BoastTimerID == 104 && g_gsi.scale == 1.0f && s.Displayed && g_gsi.shown); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d17ea0_TEST FAIL\n"); return 1; }
    std::printf("00d17ea0_TEST PASS\n");
    return 0;
}
