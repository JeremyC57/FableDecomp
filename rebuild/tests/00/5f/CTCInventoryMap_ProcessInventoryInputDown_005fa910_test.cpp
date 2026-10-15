#include <cstdio>
// Behaviour test for 0x005FA910: world map: index steps down and wraps to NumRegions below 1; otherwise the base handler.
static int g_base = 0;
struct CTCInventoryBase { virtual void v00() {} long InventorySelectionState; void ProcessInventoryInputDown() { ++g_base; } };
struct CTCInventoryMap : CTCInventoryBase {
    bool WorldMapActive; long ActiveRegionIndex; long NumRegions;
    virtual void v00() {}
    void ProcessInventoryInputDown();
};
void CTCInventoryMap::ProcessInventoryInputDown()
{
    if (InventorySelectionState == 1 && WorldMapActive) {
        if (--ActiveRegionIndex < 1)
            ActiveRegionIndex = NumRegions;
        return;
    }
    CTCInventoryBase::ProcessInventoryInputDown();
}
int main()
{
    CTCInventoryMap m; m.InventorySelectionState = 1; m.WorldMapActive = true; m.ActiveRegionIndex = 3; m.NumRegions = 5;
    m.ProcessInventoryInputDown();
    if (m.ActiveRegionIndex != 2 || g_base != 0) { std::printf("005fa910_TEST FAIL\n"); return 1; }
    m.ActiveRegionIndex = 1; m.ProcessInventoryInputDown();
    if (m.ActiveRegionIndex != 5 || g_base != 0) { std::printf("005fa910_TEST FAIL\n"); return 1; }
    m.WorldMapActive = false; m.ProcessInventoryInputDown();
    if (m.ActiveRegionIndex != 5 || g_base != 1) { std::printf("005fa910_TEST FAIL\n"); return 1; }
    m.WorldMapActive = true; m.InventorySelectionState = 0; m.ProcessInventoryInputDown();
    if (m.ActiveRegionIndex != 5 || g_base != 2) { std::printf("005fa910_TEST FAIL\n"); return 1; }
    std::printf("005fa910_TEST PASS\n");
    return 0;
}