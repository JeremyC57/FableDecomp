#pragma optimize("s", on)
// CTCInventory::ProcessInventoryInputRight() -- leaves quick-access assign and remove modes, then runs the base class's right-input handling
// Leaf: ProcessInventoryInputRight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581A54, length 25 bytes.
// Callees: CTCInventory::SetQuickAccessAssign(bool) 0x00581A14, NInventory::CTCInventoryBase::
//          ProcessInventoryInputRight() 0x005BC649 (tail jump).
// Vtable slot 41 of CTCInventory (0x01250DE4; the base vtable 0x01253DBC holds 0x005BC649 there);
// Ego_r PDB: ?ProcessInventoryInputRight@CTCInventory@@UAEXXZ follows ProcessInventoryInputLeft.
// Retail layout (engine/CTCInventory.h is the wider PDB layout): EnableQuickAccessRemove +0x1cd.
// Shape: SetQuickAccessAssign stays a call (out of line in this /Os TU, three callers), the
// one-store SetQuickAccessRemove is inlined (`mov byte ptr [esi+1CDh],0`), and the qualified base
// call is a sibling `jmp`.
struct CTCInventoryBase {
    unsigned char _pad_0x000[0x1cc];
    void ProcessInventoryInputRight();          // 0x005BC649
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;              // +0x1cc (PDB +0x204)
    bool EnableQuickAccessRemove;              // +0x1cd (PDB +0x205)
    void SetQuickAccessAssign(bool assign);    // 0x00581A14
    void SetQuickAccessRemove(bool remove) { EnableQuickAccessRemove = remove; }
    void ProcessInventoryInputRight();
};

void CTCInventory::ProcessInventoryInputRight()
{
    SetQuickAccessAssign(false);
    SetQuickAccessRemove(false);
    CTCInventoryBase::ProcessInventoryInputRight();
}
