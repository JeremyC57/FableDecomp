// Behaviour test for 0x00630350: NUISystem::CUIStateDef default ctor: no name, empty maps, graphic 0, flag 7, unit zoom, white, time -1
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)

struct Def { void* mgr; bool setup, templ, defaults; long name; void* sub[3]; bool dirty; unsigned long graphic, flag; long type; float px, py, zx, zy, r, g, b, a, time; void* children[3]; bool linear;
  Def() : mgr(0), setup(false), templ(false), defaults(false), name(-1), dirty(false) { sub[0] = sub[1] = sub[2] = 0; children[0] = children[1] = children[2] = 0; graphic = 0; px = 0.0f; py = 0.0f; type = 0; zx = 1.0f; zy = 1.0f; r = g = b = a = 1.0f; time = -1.0f; flag = 7; linear = false; } };
static void run() { Def d; CHECK(d.mgr == 0 && d.name == -1 && !d.sub[2] && !d.dirty && d.graphic == 0 && d.flag == 7 && d.type == 0 && d.px == 0 && d.zx == 1 && d.zy == 1 && d.r == 1 && d.a == 1 && d.time == -1 && !d.children[1] && !d.linear); }

int main()
{
    run();
    if (g_fail) { std::printf("00630350_TEST FAIL\n"); return 1; }
    std::printf("00630350_TEST PASS\n");
    return 0;
}
