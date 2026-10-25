// CTCInventoryMap::ProcessInventoryInputUp() -- on the world map step the active region back (wrapping to the last), else the base handling
// Leaf: ?ProcessInventoryInputUp@CTCInventoryMap@@UAEXXZ
// Retail 0x005FA910, 54 bytes (+10 int3). CTCInventoryMap vtable 0x01257A9C slot 38 (base slot 38 is
// NInventory::CTCInventoryBase::ProcessInventoryInputUp 0x005BDD8C, tail-jumped here).
// Shape: InventorySelectionState (+0x1C) == 1 and WorldMapActive (+0x234, `mov al ; test al,al`):
// `if (--ActiveRegionIndex < 1) ActiveRegionIndex = NumRegions;` (+0x238 / +0x23C) and return;
// otherwise the base handler.
// Name: base vtable slots 38 / 39 are NInventory::CTCInventoryBase::ProcessInventoryInputUp 0x005BDD8C (previous category /
// item) and ...Down 0x005BDDB8 (next); Ego_r.exe defines every class's Up before its Down (base 0x006206D3 / 0x00620703,
// CTCInventoryMap 0x0064B3A0 (48 bytes) / 0x0064B3D0 (51 bytes) against retail 0x005FA910 (54) / 0x005FA950 (59)) and retail keeps that order with Up in slot 38 in every inventory
// vtable; each override tail-jumps to the base handler of its own slot. The earlier Up/Down names of this pair were swapped.
// Retail offsets (Ego_r.pdb WorldMapActive +0x290, ActiveRegionIndex +0x294, NumRegions +0x298; the retail containers
// ahead are narrower), local model.
namespace NInventory {

class CTCInventoryBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void ProcessInventoryInputUp();           // slot 38 (0x005BDD8C)
    virtual void ProcessInventoryInputDown();         // slot 39 (0x005BDDB8)
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

void CTCInventoryMap::ProcessInventoryInputUp()
{
    if (InventorySelectionState == 1 && WorldMapActive) {
        if (--ActiveRegionIndex < 1)
            ActiveRegionIndex = NumRegions;
        return;
    }
    CTCInventoryBase::ProcessInventoryInputUp();
}
