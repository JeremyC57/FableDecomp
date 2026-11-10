// Test for 0x005FBEA0 CTCInventoryMap::GetRegionTeleporter: self-contained behaviour model -- the region's maps are searched in
// order for the first scan-visible thing with a teleporter component in that map; it is returned when its teleporter is active,
// otherwise the search goes on with the next map; 0 when no map yields one.
#include <cstdio>

struct Thing { int map; bool visible; bool hasTeleporter; bool active; };
static Thing g_things[5] = { { 1, true, false, false }, { 2, false, true, true }, { 2, true, true, false },
                             { 3, true, true, true }, { 3, true, true, true } };
static Thing* FirstTeleporterIn(int map)
{
    for (int i = 0; i < 5; ++i)
        if (g_things[i].visible && g_things[i].hasTeleporter && g_things[i].map == map)
            return &g_things[i];
    return 0;
}
static Thing* GetRegionTeleporter(const int* maps, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i) {
        Thing* teleporter = FirstTeleporterIn(maps[i]);
        if (teleporter && teleporter->active)
            return teleporter;
    }
    return 0;
}

static int fail(int n) { std::printf("005fbea0_TEST FAIL %d\n", n); return 1; }

int main()
{
    int a[3] = { 1, 2, 3 };
    if (GetRegionTeleporter(a, 3) != &g_things[3]) return fail(1);   // map 2's first teleporter is inactive: map 3 wins
    if (GetRegionTeleporter(a, 2) != 0) return fail(2);
    int b[1] = { 9 };
    if (GetRegionTeleporter(b, 1) != 0) return fail(3);
    if (GetRegionTeleporter(a, 0) != 0) return fail(4);
    std::printf("005fbea0_TEST PASS\n");
    return 0;
}