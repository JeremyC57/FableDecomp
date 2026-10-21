// Behaviour test for 0x00502850 (CWorldMap::CacheMaps): proximity load/unload decisions per map, the retail
// Maps[loadType].Loading test on the unload branch, and the immediate-load drain of the level loader.
#include <cstdio>
#include <cstring>
struct MapInfo { bool Used, Loading, Proximity, Loaded; long Distance; };
static char g_log[64]; static int g_n = 0;
static void Log(char c, long i) { g_log[g_n++] = c; g_log[g_n++] = (char)('0' + i); g_log[g_n] = 0; }
static bool g_regionLoading = false; static int g_busy = 0;
struct WorldMap {
    MapInfo Maps[4]; long Count;
    void CacheMaps(int loadType)
    {
        if (g_regionLoading) return;
        for (long i = 0; i < Count; ++i) {
            if (Maps[i].Used && Maps[i].Proximity) {
                long distance = Maps[i].Distance;
                if (Maps[i].Loaded) {
                    if (distance >= 0x40 && !Maps[loadType].Loading) { Log('M', i); Log('U', i); }
                }
                else if (!Maps[i].Loading && distance < 0x30) {
                    Log('L', i);
                    if (loadType == 0) { while (g_busy > 0) { --g_busy; Log('T', 0); } }
                }
            }
        }
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    WorldMap w; std::memset(&w, 0, sizeof w); w.Count = 4;
    MapInfo m1 = { true, false, true, true, 0x40 };  w.Maps[1] = m1;   // loaded and far: unload
    MapInfo m2 = { true, false, true, false, 0x2f }; w.Maps[2] = m2;   // unloaded and near: load
    MapInfo m3 = { true, false, false, false, 0 };   w.Maps[3] = m3;   // not proximity-loaded
    g_busy = 2; w.CacheMaps(0);
    CHECK(std::strcmp(g_log, "M1U1L2T0T0") == 0);
    g_n = 0; g_log[0] = 0; w.Maps[1].Loading = true; w.CacheMaps(1);   // Maps[1].Loading blocks the unload
    CHECK(std::strcmp(g_log, "L2") == 0);
    g_n = 0; g_log[0] = 0; g_regionLoading = true; w.CacheMaps(0);
    CHECK(g_n == 0);
    if (g_fail) { std::printf("00502850_TEST FAIL\n"); return 1; }
    std::printf("00502850_TEST PASS\n");
    return 0;
}
