#include <cstdio>
// Behaviour test for 0x005FA950: world map: index steps up and wraps to 1 above NumRegions; otherwise the base handler.
static int g_base = 0;
struct CTCInventoryBase { virtual void v00() {} long InventorySelectionState; void ProcessInventoryInputUp() { ++g_base; } };
struct CTCInventoryMap : CTCInventoryBase {
    bool WorldMapActive; long ActiveRegionIndex; long NumRegions;
    virtual void v00() {}
    void ProcessInventoryInputUp();
};
void CTCInventoryMap::ProcessInventoryInputUp()
{
    if (InventorySelectionState == 1 && WorldMapActive) {
        if (++ActiveRegionIndex > NumRegions)
            ActiveRegionIndex = 1;
        return;
    }
    CTCInventoryBase::ProcessInventoryInputUp();
}
int main()
{
    CTCInventoryMap m; m.InventorySelectionState = 1; m.WorldMapActive = true; m.ActiveRegionIndex = 4; m.NumRegions = 5;
    m.ProcessInventoryInputUp();
    if (m.ActiveRegionIndex != 5 || g_base != 0) { std::printf("005fa950_TEST FAIL\n"); return 1; }
    m.ProcessInventoryInputUp();
    if (m.ActiveRegionIndex != 1 || g_base != 0) { std::printf("005fa950_TEST FAIL\n"); return 1; }
    m.InventorySelectionState = 2; m.ProcessInventoryInputUp();
    if (m.ActiveRegionIndex != 1 || g_base != 1) { std::printf("005fa950_TEST FAIL\n"); return 1; }
    std::printf("005fa950_TEST PASS\n");
    return 0;
}
