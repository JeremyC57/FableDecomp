// CTCInventoryStats::CloseInventory() -- stops using the stats-screen mannequin, kills it if one was spawned, and marks the inventory closed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CC6F3, 40 bytes. rtti-vtables: CTCInventoryStats 0x01254ADC slot 31.
// Callee: CThing::Kill(bool) @ 0x004C9B80 (tests the dead/flag bits at +0x91/+0x92 first).
//
// Shape: a size-optimised TU (`and dword ptr [edi],0` for the null store; `#pragma optimize("s", on)`): the mannequin
// pointer is addressed once through `lea edi,[esi+140h]`, its load is scheduled above the UseMannequin byte store, the
// Kill call takes `push 0` = false. Members from the PDB: CTCInventoryStats::pMannequin (+0x15C) / UseMannequin (+0x160)
// sit 0x1C lower in retail (seven 12-byte vectors before them), CTCInventoryBase::InventoryOpen (+0x60) at +0x58.
// Local model (no engine header for CTCInventoryStats / CTCInventoryBase / CThing).
#pragma optimize("s", on)
struct CThing {
    void Kill(bool immediate);                // 0x004C9B80
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x58 - 4];
    bool InventoryOpen;                       // +0x58
    unsigned char _pad_0x59[0x130 - 0x59];
};
struct CTCInventoryStats : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x140 - 0x130];
    CThing* pMannequin;                       // +0x140
    bool UseMannequin;                        // +0x144
    void CloseInventory();
};
void CTCInventoryStats::CloseInventory()
{
    UseMannequin = false;
    if (pMannequin) {
        pMannequin->Kill(false);
        pMannequin = 0;
    }
    InventoryOpen = false;
}
