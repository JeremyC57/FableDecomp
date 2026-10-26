// CTCInventoryClothing::CloseInventory() -- clears the morph primitive handles, drops the mannequin flag, kills and forgets the mannequin thing, marks the inventory closed and clears the clothing menu entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B6A2D, length 57 bytes; size TU (`and dword ptr [edi],0` zero store, sibling `jmp`).
// Virtual slot 31 of the CTCInventoryClothing vtable 0x0124356C.
// Callees: the 12-byte retail std::map clear @ 0x004E4108 on +0x14C (manifest "CTCCarryable::OnKill" is propagated), CThing::Kill(bool)
// @ 0x004C9B80 with false, and the out-of-line ClothingMenuEntries clear @ 0x005B432C (`erase(first, last)` on the vector at +0x134;
// manifest "Unwind@00fbdab0" is wrong), tail-called.
// Model: engine/CTCInventoryClothing.h (PDB) has MorphPrimitiveHandles +0x16C, PMannequin +0x17C, UseMannequin +0x180; retail uses
// +0x14C / +0x158 / +0x15C, and the base's InventoryOpen is +0x58, so a local view carries the PDB names at the retail offsets.
// Shape: PMannequin is addressed through `lea edi,[esi+158h]` (tested, then zeroed through edi); the UseMannequin clear written
// before the `if` is scheduled between the null test and its branch.
#pragma optimize("s", on)
class CThing { public:
    void Kill(bool immediate);                                   // 0x004C9B80
};
struct CMorphPrimitiveHandleMap {                                // retail std::map<long, CEnginePrimitiveHandle> (12 bytes)
    void* m_header;
    unsigned long m_size;
    unsigned long m_spare;
    void clear();                                                // 0x004E4108
};
class CTCInventoryClothing { public:
    unsigned char            _pad_0x000[0x58];
    bool                     InventoryOpen;                      // +0x058 (CTCInventoryBase)
    unsigned char            _pad_0x059[0xF3];
    CMorphPrimitiveHandleMap MorphPrimitiveHandles;              // +0x14C
    CThing*                  PMannequin;                         // +0x158
    bool                     UseMannequin;                       // +0x15C
    void ClearClothingMenuEntries();                             // 0x005B432C
    void CloseInventory();
};
void CTCInventoryClothing::CloseInventory()
{
    MorphPrimitiveHandles.clear();
    UseMannequin = false;
    if (PMannequin) {
        PMannequin->Kill(false);
        PMannequin = 0;
    }
    InventoryOpen = false;
    ClearClothingMenuEntries();
}