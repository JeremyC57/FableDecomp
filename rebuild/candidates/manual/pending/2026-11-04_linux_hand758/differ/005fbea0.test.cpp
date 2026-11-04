// Behaviour test for CTCInventoryMap::GetRegionTeleporter (0x005FBEA0), g++ model: per region map, the first thing with a teleporter component in that map; returned when the teleporter is active.
#include <cstdio>
#include <vector>
struct Thing { long map; bool hasTeleporter, active; };
static const Thing* GetRegionTeleporter(const std::vector<long>& maps, const std::vector<Thing>& things)
{
    for (size_t i = 0; i < maps.size(); ++i) {
        const Thing* found = 0;
        for (size_t k = 0; k < things.size(); ++k) if (things[k].hasTeleporter && things[k].map == maps[i]) { found = &things[k]; break; }
        if (found && found->active) return found;
    }
    return 0;
}
int main()
{
    std::vector<Thing> t; Thing a = { 1, true, false }, b = { 1, true, true }, c = { 2, false, true }, d = { 3, true, true };
    t.push_back(a); t.push_back(b); t.push_back(c); t.push_back(d);
    std::vector<long> m; m.push_back(1); m.push_back(2); m.push_back(3);
    int ok = GetRegionTeleporter(m, t) == &t[3];        // map 1's first teleporter is inactive
    std::vector<long> m2(1, 2); ok = ok && GetRegionTeleporter(m2, t) == 0 && GetRegionTeleporter(std::vector<long>(), t) == 0;
    std::printf(ok ? "005fbea0_TEST PASS\n" : "005fbea0_TEST FAIL\n");
    return ok ? 0 : 1;
}
