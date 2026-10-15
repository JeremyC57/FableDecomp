#pragma optimize("s", on)
// CTCInventory::ProcessButtonYReleased() -- in item selection, Y toggles quick-access assign mode (off when on; on when the remove mode is off); then the base handling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581B01, length 47 bytes.
// Callees: CTCInventory::SetQuickAccessAssign(bool) 0x00581A14, NInventory::CTCInventoryBase::
//          ProcessButtonYReleased() 0x005BC692 (tail jump).
// Vtable slot 61 of CTCInventory (0x01250DE4). Ego_r PDB: ?ProcessButtonYReleased@CTCInventory@@UAEXXZ.
// Retail layout (engine/CTCInventory.h is the wider PDB layout): InventorySelectionState +0x1c,
// EnableQuickAccessAssign +0x1cc, EnableQuickAccessRemove +0x1cd.
// Shape: /Os cross-jumping merges the two SetQuickAccessAssign calls behind their different pushes
// (`push eax` = false / `push 1`) and the byte tests compare against the zeroed eax.
struct CTCInventoryBase {
    unsigned char _pad_0x000[0x1c];
    long          InventorySelectionState;           // +0x1c (PDB +0x20)
    unsigned char _pad_0x020[0x1ac];
    void ProcessButtonYReleased();                   // 0x005BC692
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;                    // +0x1cc (PDB +0x204)
    bool EnableQuickAccessRemove;                    // +0x1cd (PDB +0x205)
    void SetQuickAccessAssign(bool assign);          // 0x00581A14
    void ProcessButtonYReleased();
};

void CTCInventory::ProcessButtonYReleased()
{
    if (InventorySelectionState == 1) {
        if (EnableQuickAccessAssign)
            SetQuickAccessAssign(false);
        else if (!EnableQuickAccessRemove)
            SetQuickAccessAssign(true);
    }
    CTCInventoryBase::ProcessButtonYReleased();
}