// Behaviour test for 0x008561c0: CCombatActionBase::ResetStrikeState: timings, flags and stage reset
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct A { long LastEnd; float IntTime, ResetTime; long MoveFrame; float SpdStart, SpdEnd; bool f[9]; long Stage; bool HitThing, HitCreature; long Pause;
  void Reset() { f[6] = false; IntTime = 0.0f; f[0] = false; f[3] = false; f[5] = false; Stage = -1; f[1] = false; ResetTime = 0.0f; f[2] = false; f[4] = false; LastEnd = 0; f[7] = false; f[8] = false; SpdStart = -1.0f; SpdEnd = -1.0f; HitThing = false; HitCreature = false; Pause = 0; } };
static void run() { A a; std::memset(&a, 0x55, sizeof(a)); a.MoveFrame = 7; a.Reset(); CHECK(a.LastEnd == 0 && a.IntTime == 0.0f && a.ResetTime == 0.0f && a.MoveFrame == 7 && a.SpdStart == -1.0f && a.SpdEnd == -1.0f && a.Stage == -1 && !a.HitThing && !a.HitCreature && a.Pause == 0); for (int i = 0; i < 9; ++i) CHECK(!a.f[i]); }

int main()
{
    run();
    if (g_fail) { std::printf("008561c0_TEST FAIL\n"); return 1; }
    std::printf("008561c0_TEST PASS\n");
    return 0;
}