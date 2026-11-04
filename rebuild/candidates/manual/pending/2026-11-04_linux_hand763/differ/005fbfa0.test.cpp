// Behaviour test for 0x005FBFA0 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <vector>
struct Region { std::vector<int> Maps; };
static int g_teleported = -1, g_closed = 0;
static void TeleportToActiveRegion(int current, int active, const std::vector<Region>& regions, const std::vector<int>& teleporterMaps) {
    if (current == active) return;
    const Region& r = regions[active];
    for (size_t i = 0; i < r.Maps.size(); ++i)
        for (size_t k = 0; k < teleporterMaps.size(); ++k)
            if (teleporterMaps[k] == r.Maps[i]) { ++g_closed; g_teleported = r.Maps[i]; return; }
}
int main() {
    std::vector<Region> regions(2); regions[1].Maps.push_back(7); regions[1].Maps.push_back(8);
    std::vector<int> tele; tele.push_back(8);
    TeleportToActiveRegion(0, 0, regions, tele); bool ok = g_teleported == -1;
    TeleportToActiveRegion(0, 1, regions, tele); ok = ok && g_teleported == 8 && g_closed == 1;
    std::printf(ok ? "005fbfa0_TEST PASS\n" : "005fbfa0_TEST FAIL\n");
    return ok ? 0 : 1;
}
