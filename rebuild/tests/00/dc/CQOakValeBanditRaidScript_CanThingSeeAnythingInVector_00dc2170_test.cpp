// Behaviour test for 0x00dc2170 (CQ_OakValeBanditRaidScript::CanThingSeeAnythingInVector): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Thing { int pos; };
struct GSI { bool CanThingBeSeenByOtherThing(const Thing& seen, const Thing& looker) const { int d = seen.pos - looker.pos; return d > -3 && d < 3; } };
static long CanSee(GSI* g, const Thing& thing, Thing* v, unsigned long n) { for (unsigned long i = 0; i < n; ++i) if (g->CanThingBeSeenByOtherThing(v[i], thing)) return (long)i; return -1; }

int main()
{
    GSI g; Thing t = { 0 }; Thing v[4] = { { 10 }, { 7 }, { 2 }, { 1 } };
    CHECK(CanSee(&g, t, v, 4) == 2); CHECK(CanSee(&g, t, v, 2) == -1); CHECK(CanSee(&g, t, v, 0) == -1);
    if (g_fail) { std::printf("00dc2170_TEST FAIL\n"); return 1; }
    std::printf("00dc2170_TEST PASS\n");
    return 0;
}