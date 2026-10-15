#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCInventory::ProcessButtonAReleased() -- outside the quick-access modes and in item selection, selects the highlighted item and closes the inventory when asked to; then the base handling
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581A6D, length 70 bytes.
// Callees: NInventory::CTCInventoryBase::SelectHighlightedItem(bool&) 0x005BDD04, the virtual
//          AddCloseInventoryEvent (slot 32, `call [eax+80h]`; CTCInventory 0x00584A73), and
//          NInventory::CTCInventoryBase::ProcessButtonAReleased() 0x005BC65C (landed).
// Vtable slot 58 of CTCInventory (0x01250DE4). Ego_r PDB: ?ProcessButtonAReleased@CTCInventory@@UAEXXZ.
// Retail layout (engine/CTCInventory.h is the wider PDB layout): InventorySelectionState +0x1c (base,
// PDB +0x20), EnableQuickAccessAssign +0x1cc, EnableQuickAccessRemove +0x1cd.
// Shape: frame-pointer /Os TU (`push ecx` frame for the out bool, `leave`): `optimize("y",off)` +
// `("s",on)`; the bool is cleared before the call and tested from memory afterwards.
struct CTCInventoryBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void AddCloseInventoryEvent();           // slot 32
    unsigned char _pad_0x004[0x18];
    long          InventorySelectionState;           // +0x1c (PDB +0x20)
    unsigned char _pad_0x020[0x1ac];
    void SelectHighlightedItem(bool& closeInventory);    // 0x005BDD04
    void ProcessButtonAReleased();                   // 0x005BC65C
};

struct CTCInventory : CTCInventoryBase {
    bool EnableQuickAccessAssign;                    // +0x1cc (PDB +0x204)
    bool EnableQuickAccessRemove;                    // +0x1cd (PDB +0x205)
    void ProcessButtonAReleased();
};

void CTCInventory::ProcessButtonAReleased()
{
    if (!EnableQuickAccessAssign && !EnableQuickAccessRemove && InventorySelectionState == 1) {
        bool closeInventory = false;
        SelectHighlightedItem(closeInventory);
        if (closeInventory)
            AddCloseInventoryEvent();
    }
    CTCInventoryBase::ProcessButtonAReleased();
}