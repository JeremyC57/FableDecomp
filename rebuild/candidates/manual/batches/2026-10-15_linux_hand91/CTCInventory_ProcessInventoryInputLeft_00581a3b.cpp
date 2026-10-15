#pragma optimize("s", on)
// CTCInventory::ProcessInventoryInputLeft() -- leaves quick-access assign and remove modes, then runs the base class's left-input handling
// Leaf: ProcessInventoryInputLeft
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581A3B, length 25 bytes.
// Callees: CTCInventory::SetQuickAccessAssign(bool) 0x00581A14, NInventory::CTCInventoryBase::
//          ProcessInventoryInputLeft() 0x005BC638 (tail jump).
// Vtable slot 40 of CTCInventory (0x01250DE4; the base vtable 0x01253DBC holds 0x005BC638 there);
// Ego_r PDB: ?ProcessInventoryInputLeft@CTCInventory@@UAEXXZ follows SetQuickAccessRemove.
// Retail layout (engine/CTCInventory.h is the wider PDB layout): EnableQuickAccessRemove +0x1cd.
// Shape: SetQuickAccessAssign stays a call (out of line in this /Os TU, three callers), the
// one-store SetQuickAccessRemove is inlined (`mov byte ptr [esi+1CDh],0`), and the qualified base
// call is a sibling `jmp`.
struct CTCInventoryBase {
    unsigned char _pad_0x000[0x1cc];
    void ProcessInventoryInputLeft();          // 0x005BC638
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;              // +0x1cc (PDB +0x204)
    bool EnableQuickAccessRemove;              // +0x1cd (PDB +0x205)
    void SetQuickAccessAssign(bool assign);    // 0x00581A14
    void SetQuickAccessRemove(bool remove) { EnableQuickAccessRemove = remove; }
    void ProcessInventoryInputLeft();
};

void CTCInventory::ProcessInventoryInputLeft()
{
    SetQuickAccessAssign(false);
    SetQuickAccessRemove(false);
    CTCInventoryBase::ProcessInventoryInputLeft();
}
