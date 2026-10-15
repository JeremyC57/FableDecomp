// Behaviour test for 0x0043a4d0: SetGuildSealLevelUpPos: push a non-zero position, clear on zero.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static float g_eps = 0.0001f;
struct V2 { float X, Y; };
struct Queue { V2 items[8]; int n; void push_back(const V2& v) { items[n++] = v; } void clear() { n = 0; } };
struct Gui { Queue q;
  void Set(const V2& p) { float y = p.Y, x = p.X; if (x * x + y * y > g_eps * g_eps) q.push_back(p); else q.clear(); } };
static int run() { Gui g; g.q.n = 0; V2 a = {1, 2}, b = {0, 0}, c = {0.00001f, 0}; g.Set(a); g.Set(a); CHECK(g.q.n == 2 && g.q.items[1].Y == 2);
  g.Set(c); CHECK(g.q.n == 0); g.Set(a); g.Set(b); CHECK(g.q.n == 0); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("0043a4d0_TEST FAIL\n"); return 1; }
    std::printf("0043a4d0_TEST PASS\n");
    return 0;
}