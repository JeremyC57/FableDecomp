// Behaviour test for 0x006337c0: InitModeParalysed: six processes attached to the interface.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Iface { int dummy; };
struct Proc { int added; Iface* to; long mode; void* cam; void Add(Iface* i) { ++added; to = i; } };

struct Player { Iface* PlayerInterface; Proc *par, *rs, *look, *cyc, *mov, *dbg;
  void Init() { par->Add(PlayerInterface); rs->Add(PlayerInterface); look->Add(PlayerInterface); cyc->Add(PlayerInterface); mov->Add(PlayerInterface); dbg->Add(PlayerInterface); } };
static int run() { Iface i; Proc p[6]; for (int k = 0; k < 6; ++k) { p[k].added = 0; p[k].to = 0; }
  Player pl = {&i, &p[0], &p[1], &p[2], &p[3], &p[4], &p[5]}; pl.Init();
  for (int k = 0; k < 6; ++k) CHECK(p[k].added == 1 && p[k].to == &i); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("006337c0_TEST FAIL\n"); return 1; }
    std::printf("006337c0_TEST PASS\n");
    return 0;
}
