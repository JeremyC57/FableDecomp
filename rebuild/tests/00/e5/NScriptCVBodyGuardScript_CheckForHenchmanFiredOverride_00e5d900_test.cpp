// Behaviour test for 0x00e5d900: CV_BodyGuardScript::CheckForHenchmanFiredOverride: state 2 waits until no henchmen remain, then clears to 0
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static int g_frames = 0; static int g_henchmen = 2; static int g_terminate_at = 100;
struct Data { long state; };
struct Script { Data* data; bool Terminating() const { return g_frames >= g_terminate_at; } void Frame() { ++g_frames; if (g_henchmen > 0 && g_frames >= 3) --g_henchmen; }
  void Check() { while (!Terminating()) { if (data->state == 2) { if (Terminating()) return; while (g_henchmen != 0) { Frame(); if (Terminating()) return; } if (Terminating()) return; data->state = 0; } Frame(); } } };
static void run() { Data d; d.state = 2; Script s; s.data = &d; g_terminate_at = 10; s.Check(); CHECK(d.state == 0 && g_henchmen == 0 && g_frames == 10);
  g_frames = 0; g_henchmen = 5; d.state = 2; g_terminate_at = 2; s.Check(); CHECK(d.state == 2); }

int main()
{
    run();
    if (g_fail) { std::printf("00e5d900_TEST FAIL\n"); return 1; }
    std::printf("00e5d900_TEST PASS\n");
    return 0;
}