// Behaviour test for CWorldMap::UnloadRegion (0x004FEEC0): a model of the row -- with RegionUnloading set, every loaded map of the region is deactivated, dropped from the engine and the loaded-map list and its counted map pointer released; then every map of the region leaves the thing-map manager and (when loaded) the navigator; LoadedRegion becomes 0 and message 0x27 carries the region.
#include <stdio.h>
#include <vector>
#include <algorithm>
struct Info { long count; };
struct MapInfo { bool Activated; Info* PMap; };
struct World {
    std::vector<MapInfo> Maps; std::vector<std::vector<long> > Regions; std::vector<long> LoadedMaps;
    std::vector<long> NavLoaded, ThingMapRemoved, EngineRemoved; long LoadedRegion; bool RegionUnloading, SeenUnloading;
    long MsgType, MsgExtra;
    bool IsMapLoaded(long m) const { return std::find(LoadedMaps.begin(), LoadedMaps.end(), m) != LoadedMaps.end(); }
    void UnloadRegion(long region) {
        std::vector<long>& maps = Regions[region];
        RegionUnloading = true; SeenUnloading = RegionUnloading;
        for (unsigned int i = 0; i < maps.size(); ++i) { long map = maps[i];
            if (IsMapLoaded(map)) { Maps[map].Activated = false; EngineRemoved.push_back(map);
                LoadedMaps.erase(std::find(LoadedMaps.begin(), LoadedMaps.end(), map));
                if (Maps[map].PMap && --Maps[map].PMap->count == 0) { delete Maps[map].PMap; } Maps[map].PMap = 0; } }
        for (unsigned int j = 0; j < maps.size(); ++j) { long map = maps[j]; ThingMapRemoved.push_back(map);
            std::vector<long>::iterator n = std::find(NavLoaded.begin(), NavLoaded.end(), map); if (n != NavLoaded.end()) NavLoaded.erase(n); }
        LoadedRegion = 0; MsgType = 0x27; MsgExtra = region; RegionUnloading = false; }
};
int main() { World w; w.Maps.resize(4); for (int i = 0; i < 4; ++i) { w.Maps[i].Activated = true; w.Maps[i].PMap = new Info(); w.Maps[i].PMap->count = 1; }
  w.Regions.resize(2); w.Regions[1].push_back(1); w.Regions[1].push_back(3); w.LoadedMaps.push_back(0); w.LoadedMaps.push_back(1);
  w.NavLoaded.push_back(3); w.LoadedRegion = 1; w.UnloadRegion(1);
  bool ok = !w.Maps[1].Activated && w.Maps[1].PMap == 0 && w.Maps[3].Activated && w.Maps[3].PMap != 0 && w.LoadedMaps.size() == 1
         && w.EngineRemoved.size() == 1 && w.ThingMapRemoved.size() == 2 && w.NavLoaded.empty() && w.LoadedRegion == 0
         && w.MsgType == 0x27 && w.MsgExtra == 1 && w.SeenUnloading && !w.RegionUnloading;
  printf(ok ? "004feec0_TEST PASS\n" : "004feec0_TEST FAIL\n");
  return ok ? 0 : 1;
}