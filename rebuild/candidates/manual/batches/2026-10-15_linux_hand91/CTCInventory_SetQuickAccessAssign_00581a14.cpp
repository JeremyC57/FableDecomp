#pragma optimize("s", on)
// CTCInventory::SetQuickAccessAssign(bool assign) -- enters/leaves quick-access assign mode; the mode only switches on while the highlighted item can be quick-assigned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581A14, length 26 bytes.
// Callee: NInventory::CTCInventoryBase::GetCanItemBeQuickAssigned() 0x005BC501 (reads the base's
//         CanItemBeQuickAssigned byte +0x34).
// Ego_r PDB: ?SetQuickAccessAssign@CTCInventory@@QAEX_N@Z. engine/CTCInventory.h is the PDB layout,
// whose std containers are 4 bytes wider than retail's: EnableQuickAccessAssign (PDB +0x204) sits at
// retail +0x1cc, so the class is modelled locally.
// Shape: `cond ? assign : false` is the branch-free mask (`neg al ; sbb al,al ; and eax,[esp+8]`);
// size-optimised TU (odd address).
struct CTCInventoryBase {
    unsigned char _pad_0x000[0x1cc];
    bool GetCanItemBeQuickAssigned();          // 0x005BC501
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;              // +0x1cc (PDB +0x204)
    bool EnableQuickAccessRemove;              // +0x1cd (PDB +0x205)
    void SetQuickAccessAssign(bool assign);
};

void CTCInventory::SetQuickAccessAssign(bool assign)
{
    EnableQuickAccessAssign = GetCanItemBeQuickAssigned() ? assign : false;
}
