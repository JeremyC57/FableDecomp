// Behaviour test for 0x005FA950 CTCInventoryMap::ProcessInventoryInputDown: the method comes from the source object.
#include <cstdio>
#include <stddef.h>
static int g_baseUp = 0, g_baseDown = 0;
namespace NInventory {

class CTCInventoryBase {
public:
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void v04() {}
    virtual void v05() {}
    virtual void v06() {}
    virtual void v07() {}
    virtual void v08() {}
    virtual void v09() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual void v16() {}
    virtual void v17() {}
    virtual void v18() {}
    virtual void v19() {}
    virtual void v20() {}
    virtual void v21() {}
    virtual void v22() {}
    virtual void v23() {}
    virtual void v24() {}
    virtual void v25() {}
    virtual void v26() {}
    virtual void v27() {}
    virtual void v28() {}
    virtual void v29() {}
    virtual void v30() {}
    virtual void v31() {}
    virtual void v32() {}
    virtual void v33() {}
    virtual void v34() {}
    virtual void v35() {}
    virtual void v36() {}
    virtual void v37() {}
    virtual void ProcessInventoryInputUp() { ++g_baseUp; }           // slot 38 (0x005BDD8C)
    virtual void ProcessInventoryInputDown() { ++g_baseDown; }         // slot 39 (0x005BDDB8)
    unsigned char _base_04[0x18];
    long InventorySelectionState;                  // +0x01C
    unsigned char _base_20[0x110];                 // .. +0x130
};

} // namespace NInventory

class CTCInventoryMap : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryMap@@, vtable 0x01257A9C
public:
    unsigned char _pad_130[0x104];
    bool WorldMapActive;                           // +0x234
    long ActiveRegionIndex;                        // +0x238
    long NumRegions;                               // +0x23C
    virtual void ProcessInventoryInputUp();        // slot 38 (0x005FA910)
    virtual void ProcessInventoryInputDown();      // slot 39 (0x005FA950)
};
void CTCInventoryMap::ProcessInventoryInputUp() { }
int main()
{
    int ok = 1;
    CTCInventoryMap m; m.InventorySelectionState = 1; m.WorldMapActive = true; m.ActiveRegionIndex = 4; m.NumRegions = 5;
    m.ProcessInventoryInputDown(); ok &= m.ActiveRegionIndex == 5 && g_baseDown == 0;
    m.ProcessInventoryInputDown(); ok &= m.ActiveRegionIndex == 1 && g_baseDown == 0;
    m.InventorySelectionState = 2; m.ProcessInventoryInputDown(); ok &= m.ActiveRegionIndex == 1 && g_baseDown == 1;
    ok &= g_baseUp == 0;
    ok &= offsetof(CTCInventoryMap, WorldMapActive) == 0x234 && offsetof(CTCInventoryMap, NumRegions) == 0x23c;
    if (!ok) { std::printf("005fa950_TEST FAIL\n"); return 1; }
    std::printf("005fa950_TEST PASS\n");
    return 0;
}
