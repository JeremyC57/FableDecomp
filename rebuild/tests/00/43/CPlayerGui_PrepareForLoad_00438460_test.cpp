// Behaviour test for 0x00438460: PrepareForLoad: illustrators prepared, displays reset, live GUI left, default flags.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Gui { int illPrepared, seal, quest, sub, toggles; bool minimapActive, live; bool closed, time, bag, msg, sub2, bottom;
  void Prepare() { ++illPrepared; ++seal; ++quest; minimapActive = true; ++sub; if (live) { ++toggles; live = false; } closed = false; time = true; bag = false; msg = true; sub2 = false; bottom = true; } };
static int run() { Gui g = {0,0,0,0,0,false,true,true,false,true,false,true,false}; g.Prepare();
  CHECK(g.illPrepared == 1 && g.toggles == 1 && !g.live && g.minimapActive && !g.closed && g.time && !g.bag && g.msg && !g.sub2 && g.bottom);
  g.Prepare(); CHECK(g.toggles == 1); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("00438460_TEST FAIL\n"); return 1; }
    std::printf("00438460_TEST PASS\n");
    return 0;
}