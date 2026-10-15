#include <cstdio>
// Behaviour test for 0x005FAA30: the map is shown and the active region comes from world -> region map -> index.
struct CWorldRegionMap { long index; long GetActiveRegionIndex() { return index; } };
struct CWorld { CWorldRegionMap* map; virtual CWorldRegionMap* GetRegionMap() { return map; } };
static CWorld* g_world = 0;
struct CTCInventoryBase { virtual void v00() {} CWorld* GetWorld() const { return g_world; } };
struct CTCInventoryMap : CTCInventoryBase {
    bool WorldMapActive; long ActiveRegionIndex; long NumRegions;
    virtual void v00() {}
    void ResetWorldMapDisplay();
};
void CTCInventoryMap::ResetWorldMapDisplay()
{
    WorldMapActive = true;
    ActiveRegionIndex = GetWorld()->GetRegionMap()->GetActiveRegionIndex();
}
int main()
{
    CWorldRegionMap rm; rm.index = 12; CWorld w; w.map = &rm; g_world = &w;
    CTCInventoryMap m; m.WorldMapActive = false; m.ActiveRegionIndex = 0;
    m.ResetWorldMapDisplay();
    if (!m.WorldMapActive || m.ActiveRegionIndex != 12) { std::printf("005faa30_TEST FAIL\n"); return 1; }
    std::printf("005faa30_TEST PASS\n");
    return 0;
}
