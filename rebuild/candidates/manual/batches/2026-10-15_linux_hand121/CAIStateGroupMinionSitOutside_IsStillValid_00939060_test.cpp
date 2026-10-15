#include <cstdio>
// MinionSitOutside::IsStillValid: false during the village curfew, else frame <= FrameToStopSitting
static bool g_hasVillager = true; static bool g_hasVillage = true; static bool g_curfew = false; static long g_frame = 10;
struct Group {
    long FrameToStopSitting;
    unsigned char IsStillValid()
    {
        if (g_hasVillager) { if (g_hasVillage) { if (g_curfew) return 0; } }
        return g_frame <= FrameToStopSitting;
    }
};
int main()
{
    Group g; g.FrameToStopSitting = 10;
    if (!g.IsStillValid()) { std::printf("00939060_TEST FAIL\n"); return 1; }
    g_frame = 11; if (g.IsStillValid()) { std::printf("00939060_TEST FAIL\n"); return 1; }
    g_frame = 5; g_curfew = true; if (g.IsStillValid()) { std::printf("00939060_TEST FAIL\n"); return 1; }
    g_hasVillage = false; if (!g.IsStillValid()) { std::printf("00939060_TEST FAIL\n"); return 1; }
    g_hasVillage = true; g_hasVillager = false; if (!g.IsStillValid()) { std::printf("00939060_TEST FAIL\n"); return 1; }
    std::printf("00939060_TEST PASS\n"); return 0;
}
