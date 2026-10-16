#include <cstdio>
// Behaviour model of CTCHeroStats::GetWivesInMarriageState (0x0057AE53): clears the out list, then collects every
// married wife that exists and is a creature (type byte 1) whose marriage state matches (state 0 = any).
struct Thing { unsigned char type; };
struct Marriage { int wife; long state; };
static Thing g_things[4];
static Thing* Lookup(int uid) { return uid >= 0 && uid < 4 ? &g_things[uid] : 0; }
static int Collect(const Marriage* m, int n, long state, Thing** out)
{
    int count = 0;                                    // clear()
    for (int i = 0; i < n; ++i) {
        if (state == 0 || m[i].state == state) {
            Thing* wife = Lookup(m[i].wife);
            if (wife && wife->type == 1)
                out[count++] = wife;
        }
    }
    return count;
}
int main()
{
    g_things[0].type = 1; g_things[1].type = 2; g_things[2].type = 1; g_things[3].type = 1;
    Marriage m[5] = { {0, 1}, {1, 1}, {2, 2}, {9, 1}, {3, 1} };
    Thing* out[8];
    if (Collect(m, 5, 1, out) != 2 || out[0] != &g_things[0] || out[1] != &g_things[3]) { std::printf("0057ae53_TEST FAIL\n"); return 1; }
    if (Collect(m, 5, 0, out) != 3 || out[1] != &g_things[2]) { std::printf("0057ae53_TEST FAIL\n"); return 1; }
    if (Collect(m, 5, 3, out) != 0) { std::printf("0057ae53_TEST FAIL\n"); return 1; }
    std::printf("0057ae53_TEST PASS\n");
    return 0;
}
