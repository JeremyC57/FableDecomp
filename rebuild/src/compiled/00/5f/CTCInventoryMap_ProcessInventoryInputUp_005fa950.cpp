// CTCInventoryMap::ProcessInventoryInputUp() -- on the world map step the active region forward (wrapping to 1), else the base handling
// Retail 0x005FA950, 59 bytes (+5 int3). CTCInventoryMap vtable 0x01257A9C slot 39 (base slot 39 is
// 0x005BDDB8, tail-jumped here; slot 38 = ProcessInventoryInputDown 0x005FA910 steps backwards, so
// this is named the Up twin -- the manifest only has "vfunc_39").
// Shape: the constant 1 lives in edx (`mov edx,1 ; cmp eax,edx` -- it is stored again on the wrap);
// InventorySelectionState (+0x1C) == 1 and WorldMapActive (+0x234): `if (++ActiveRegionIndex >
// NumRegions) ActiveRegionIndex = 1;` (+0x238 / +0x23C, NumRegions held in esi) and return; otherwise
// the base handler. Retail offsets (Ego_r.pdb +0x290/+0x294/+0x298), local model.

// Leaf: ProcessInventoryInputUp

struct CTCInventoryBase {
    virtual void v00();
    unsigned char _base_04[0x18];
    long InventorySelectionState;                  // +0x01C
    unsigned char _base_20[0x110];                 // .. +0x130
    void ProcessInventoryInputUp();                // 0x005BDDB8
};

struct CTCInventoryMap : CTCInventoryBase {
    unsigned char _pad_130[0x104];
    bool WorldMapActive;                           // +0x234
    long ActiveRegionIndex;                        // +0x238
    long NumRegions;                               // +0x23C
    virtual void v00();
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