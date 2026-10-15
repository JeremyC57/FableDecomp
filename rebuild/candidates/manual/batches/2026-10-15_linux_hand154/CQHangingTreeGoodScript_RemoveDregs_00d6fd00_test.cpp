// Behaviour test for 0x00d6fd00 (CQ_HangingTreeGoodScript::RemoveDregs): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
static int g_faded = 0, g_destroyed = 0;
struct Thing { bool alive; };
static void FadeOutAndKillEntity(const Thing& t, float time, bool, bool) { if (t.alive && time == 0.5f) ++g_faded; }
struct Vec { Thing items[8]; int n; bool empty() const { return n == 0; } Thing& back() { return items[n - 1]; } void pop_back() { --n; ++g_destroyed; } };
static void RemoveDregs(Vec& v) { while (!v.empty()) { if (v.back().alive) FadeOutAndKillEntity(v.back(), 0.5f, true, true); v.pop_back(); } }

int main()
{
    Vec v; v.n = 5; for (int i = 0; i < 5; ++i) v.items[i].alive = (i % 2) == 0;
    RemoveDregs(v);
    CHECK(v.n == 0 && g_faded == 3 && g_destroyed == 5);
    if (g_fail) { std::printf("00d6fd00_TEST FAIL\n"); return 1; }
    std::printf("00d6fd00_TEST PASS\n");
    return 0;
}
