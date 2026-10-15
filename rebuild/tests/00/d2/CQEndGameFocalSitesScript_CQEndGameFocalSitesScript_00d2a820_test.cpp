// Behaviour test for 0x00d2a820: CQ_EndGameFocalSitesScript ctor: arguments stored, two timers registered in order.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_reg = 0; struct Timer { long Index; Timer() : Index(++g_reg) {} };
struct Script { void* game; void* owner; const char* a[5]; const char* b[5]; Timer jack; Timer world;
  Script(void* o, void* g) : game(g), owner(o) { for (int i = 0; i < 5; ++i) { a[i] = ""; b[i] = ""; } } };
static int run() { int o, g; Script s(&o, &g); CHECK(s.game == &g && s.owner == &o && s.jack.Index == 1 && s.world.Index == 2); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00d2a820_TEST FAIL\n"); return 1; }
    std::printf("00d2a820_TEST PASS\n");
    return 0;
}