// Behaviour test for 0x006335e0: InitModeControlFreeCamera: flag, attach, camera pointer, three camera copies.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Iface { int dummy; };
struct Proc { int added; Iface* to; long mode; void* cam; void Add(Iface* i) { ++added; to = i; } };

struct Cam { unsigned int w[14]; };
struct World { Cam c; const Cam& Get(Cam& s) { s = c; return s; } };
struct Player { Iface* PI; World* world; Cam cur, fre, old; Proc* fc; bool controlling;
  void Init() { controlling = true; fc->Add(PI); fc->cam = &cur; Cam s; const Cam& c = world->Get(s); cur = c; fre = c; old = c; } };
static int run() { Iface i; Proc p = {0,0,0,0}; World w; for (int k = 0; k < 14; ++k) w.c.w[k] = k * 3;
  Player pl; pl.PI = &i; pl.world = &w; pl.fc = &p; pl.controlling = false; pl.Init();
  CHECK(pl.controlling && p.added == 1 && p.cam == &pl.cur && pl.cur.w[13] == 39 && pl.fre.w[5] == 15 && pl.old.w[0] == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("006335e0_TEST FAIL\n"); return 1; }
    std::printf("006335e0_TEST PASS\n");
    return 0;
}