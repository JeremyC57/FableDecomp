// Behaviour test for 0x006e9140: CProng::SetBlocked: timer = fps*3, occupier released unless dead.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Thing { bool dead; };
struct IP { Thing* p; Thing* Get() { return p; } void Set(Thing* t) { p = t; } };
static long g_fps = 30;
struct Prong { bool Blocked; long Timer; IP Occ;
  void SetBlocked(bool b) { Blocked = b; if (b) { Timer = (long)(g_fps * 3.0f); if (Occ.Get() && !Occ.Get()->dead) { IP tmp; tmp.Set(0); Occ.Set(tmp.Get()); } } else Timer = 0; } };
static int run() { Thing live = {false}, dead = {true};
  Prong p = {false, 5, {&live}}; p.SetBlocked(true); CHECK(p.Blocked && p.Timer == 90 && p.Occ.p == 0);
  Prong q = {false, 5, {&dead}}; q.SetBlocked(true); CHECK(q.Occ.p == &dead);
  q.SetBlocked(false); CHECK(!q.Blocked && q.Timer == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("006e9140_TEST FAIL\n"); return 1; }
    std::printf("006e9140_TEST PASS\n");
    return 0;
}
