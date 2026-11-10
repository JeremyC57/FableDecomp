// Test for 0x005FBFA0 CTCInventoryMap::TeleportToActiveRegion: self-contained behaviour model -- nothing happens when the
// highlighted region is the loaded one; otherwise the first map holding a scan-visible teleporter thing wins: the "opened from
// the map" flag is cleared, the close-inventory event is queued and the hero is teleported through it (no active test here).
#include <cstdio>

static bool g_openedFromMap = true;
struct Thing { int map; bool hasTeleporter; int teleports; };
static Thing g_things[3] = { { 5, false, 0 }, { 6, true, 0 }, { 7, true, 0 } };
struct Map {
    int activeRegion, loadedRegion, closeEvents;
    void TeleportToActiveRegion(const int* maps, unsigned int count)
    {
        if (loadedRegion != activeRegion) {
            for (unsigned int i = 0; i < count; ++i) {
                Thing* found = 0;
                for (int t = 0; t < 3 && !found; ++t)
                    if (g_things[t].hasTeleporter && g_things[t].map == maps[i]) found = &g_things[t];
                if (found) {
                    g_openedFromMap = false;
                    ++closeEvents;
                    ++found->teleports;
                    return;
                }
            }
        }
    }
};

static int fail(int n) { std::printf("005fbfa0_TEST FAIL %d\n", n); return 1; }

int main()
{
    int maps[3] = { 5, 7, 6 };
    Map m = { 2, 2, 0 };
    m.TeleportToActiveRegion(maps, 3);
    if (m.closeEvents != 0 || !g_openedFromMap) return fail(1);
    m.loadedRegion = 1;
    m.TeleportToActiveRegion(maps, 3);
    if (m.closeEvents != 1 || g_openedFromMap || g_things[2].teleports != 1 || g_things[1].teleports != 0) return fail(2);
    int none[1] = { 5 };
    m.TeleportToActiveRegion(none, 1);
    if (m.closeEvents != 1) return fail(3);
    std::printf("005fbfa0_TEST PASS\n");
    return 0;
}