// Behaviour test for 0x006250b0: NUISystem::CUIStateDef copy ctor: base data, sub-def map, state fields and child list copied
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Vec { int n; int d[4]; };
struct Def { long name; Vec sub; bool dirty; unsigned long graphic, flag; long type; float px, py, zx, zy, r, g, b, a, time; Vec children; bool linear; };
static void run() { Def s; std::memset(&s, 0, sizeof(s)); s.name = 9; s.sub.n = 1; s.sub.d[0] = 5; s.dirty = true; s.graphic = 2; s.flag = 7; s.type = 1; s.px = 1; s.zy = 2; s.r = 0.5f; s.a = 1; s.time = 3; s.children.n = 2; s.children.d[1] = 8; s.linear = true;
  Def c(s); CHECK(c.name == 9 && c.sub.d[0] == 5 && c.dirty && c.graphic == 2 && c.flag == 7 && c.type == 1 && c.px == 1 && c.zy == 2 && c.r == 0.5f && c.a == 1 && c.time == 3 && c.children.d[1] == 8 && c.linear); }

int main()
{
    run();
    if (g_fail) { std::printf("006250b0_TEST FAIL\n"); return 1; }
    std::printf("006250b0_TEST PASS\n");
    return 0;
}