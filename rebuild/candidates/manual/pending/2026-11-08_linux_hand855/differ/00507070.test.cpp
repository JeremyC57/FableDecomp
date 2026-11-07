// Behaviour model of CWorldMap::BuildStaticMapGenerationVisibilityInfo (0x00507070): rebuild the sea body list (every region whose
// def has a sea: its name, its own maps, then its visible maps); building retail static maps forces map optimisations on; with them
// on, record the global def's estimated camera height and give every map of every region the set of maps visible from it (the
// region's own and visible maps) when the map loads on player proximity, else an empty set; with them off, clear the map list.
#include <cstdio>
#include <set>
#include <string>
#include <vector>
struct Region { std::string Name; bool HasSea; std::vector<long> Maps, VisMaps; };
struct SeaBody { std::string Identifier; std::vector<long> Maps; };
struct Info { std::vector<std::set<long> > Maps; std::vector<SeaBody> SeaBodies; float Height; };
static bool g_build = false, g_optimise = false;
static void Build(const std::vector<Region>& regions, const std::vector<bool>& proximity, float height, Info& info)
{
    info.SeaBodies.clear();
    for (size_t r = 0; r < regions.size(); ++r) {
        if (!regions[r].HasSea) continue;
        SeaBody desc; desc.Identifier = regions[r].Name;
        desc.Maps = regions[r].Maps;
        desc.Maps.insert(desc.Maps.end(), regions[r].VisMaps.begin(), regions[r].VisMaps.end());
        info.SeaBodies.push_back(desc);
    }
    if (g_build) g_optimise = true;
    if (!g_optimise) { info.Maps.clear(); return; }
    info.Height = height;
    info.Maps.clear(); info.Maps.resize(proximity.size());
    for (size_t r = 0; r < regions.size(); ++r)
        for (size_t m = 0; m < regions[r].Maps.size(); ++m) {
            std::set<long> visible;
            long map = regions[r].Maps[m];
            if (proximity[map]) {
                visible.insert(regions[r].Maps.begin(), regions[r].Maps.end());
                visible.insert(regions[r].VisMaps.begin(), regions[r].VisMaps.end());
            }
            info.Maps[map] = visible;
        }
}
int main()
{
    std::vector<Region> regions(2);
    regions[0].Name = "Sea"; regions[0].HasSea = true; regions[0].Maps.push_back(0); regions[0].Maps.push_back(1); regions[0].VisMaps.push_back(2);
    regions[1].Name = "Land"; regions[1].HasSea = false; regions[1].Maps.push_back(2); regions[1].VisMaps.push_back(1);
    std::vector<bool> proximity(3, true); proximity[1] = false;
    Info info; info.Height = 0.0f;
    Build(regions, proximity, 50.0f, info);
    if (info.SeaBodies.size() != 1 || info.SeaBodies[0].Maps.size() != 3 || !info.Maps.empty()) { std::printf("00507070_TEST FAIL off\n"); return 1; }
    g_build = true;
    Build(regions, proximity, 50.0f, info);
    if (!g_optimise || info.Height != 50.0f || info.Maps.size() != 3) { std::printf("00507070_TEST FAIL on\n"); return 1; }
    if (info.Maps[0].size() != 3 || !info.Maps[1].empty() || info.Maps[2].size() != 2) { std::printf("00507070_TEST FAIL visibility\n"); return 1; }
    std::printf("00507070_TEST PASS\n");
    return 0;
}
