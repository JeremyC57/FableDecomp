// Behaviour test for 0x00cfa660: CQ_ArenaScript ctor: arguments stored, vector empty, timer registered.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static long g_reg = 0; struct Timer { long Index; Timer() : Index(++g_reg) {} };
struct Thing { void* d; void* i; Thing() : d(0), i(0) {} };
struct Arena { void* game; void* owner; const char* tags[20]; void* r0; void* r1; void* r2; Thing cells; Timer crowd; Thing whisper;
  Arena(void* o, void* g) : game(g), owner(o), r0(0), r1(0), r2(0) { for (int i = 0; i < 20; ++i) tags[i] = ""; } };
static int run() { int a, b; Arena x(&a, &b); CHECK(x.game == &b && x.owner == &a && !x.r0 && !x.r1 && !x.r2 && x.crowd.Index == 1 && !x.cells.d && !x.whisper.i && x.tags[19][0] == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00cfa660_TEST FAIL\n"); return 1; }
    std::printf("00cfa660_TEST PASS\n");
    return 0;
}
