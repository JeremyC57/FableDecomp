// CTCInventoryMap::ProcessInventoryInputDown() -- on the world map step the active region back (wrapping to the last), else the base handling
// Retail 0x005FA910, 54 bytes (+10 int3). CTCInventoryMap vtable 0x01257A9C slot 38 (the slot whose
// base is CTCInventoryBase::ProcessInventoryInputDown 0x005BDD8C, tail-jumped here).
// Shape: InventorySelectionState (+0x1C) == 1 and WorldMapActive (+0x234, `mov al ; test al,al`):
// `if (--ActiveRegionIndex < 1) ActiveRegionIndex = NumRegions;` (+0x238 / +0x23C) and return;
// otherwise the base handler. Retail offsets (Ego_r.pdb WorldMapActive +0x290, ActiveRegionIndex
// +0x294, NumRegions +0x298; the retail containers ahead are narrower), local model.

// Leaf: ProcessInventoryInputDown

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base_04[0x18];
    long InventorySelectionState;                  // +0x01C
    unsigned char _base_20[0x110];                 // .. +0x130
    void ProcessInventoryInputDown();              // 0x005BDD8C
};

struct CTCInventoryMap : CTCInventoryBase {
    unsigned char _pad_130[0x104];
    bool WorldMapActive;                           // +0x234
    long ActiveRegionIndex;                        // +0x238
    long NumRegions;                               // +0x23C
    virtual void v00();
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