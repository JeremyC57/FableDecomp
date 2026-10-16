// Behaviour test for 0x00ea12e0: CV_KnotholeGladeGatesScript::WatchForGates: close and open requests acted on once and cleared
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

static int g_frames = 0, g_closed = 0, g_opened = 0;
struct Data { bool closeReq, openReq; };
struct Script { Data* data; bool Terminating() const { return g_frames >= 3; }
  void Watch() { while (!Terminating()) { if (data->closeReq) { if (Terminating()) return; ++g_closed; data->closeReq = false; } if (data->openReq) { if (Terminating()) return; ++g_opened; data->openReq = false; } ++g_frames; if (g_frames == 1) data->openReq = true; } } };
static void run() { Data d; d.closeReq = true; d.openReq = false; Script s; s.data = &d; s.Watch(); CHECK(g_closed == 1 && g_opened == 1 && !d.closeReq && !d.openReq && g_frames == 3); }

int main()
{
    run();
    if (g_fail) { std::printf("00ea12e0_TEST FAIL\n"); return 1; }
    std::printf("00ea12e0_TEST PASS\n");
    return 0;
}
