// CTCInventoryWeapons::ProcessButtonAReleased() -- in item-selection state selects the highlighted item and, if one was selected, raises the close-inventory event; then runs the base handler
// Leaf: ProcessButtonAReleased@CTCInventoryWeapons
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C2E2A, length 52 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (one-byte `selected` local at [ebp-1] in a `push ecx` frame). Virtual slot 58 of the CTCInventoryWeapons vtable 0x01253FF4.
// The catalog row is labelled CTCInventoryClothing::ProcessButtonAReleased (propagated from the byte-twin 0x005B34D5); the only
// reference to this body is slot 58 of the CTCInventoryWeapons vtable, so it is the weapons screen's override.
// Callees: CTCInventoryBase::SelectHighlightedItem(bool&) @ 0x005BDD04, virtual slot 32 (+0x80, AddCloseInventoryEvent @ 0x005BC527
// in these vtables), CTCInventoryBase::ProcessButtonAReleased @ 0x005BC65C (the explicit base call).
// Model: engine/CTCInventoryWeapons.h (PDB) only names the derived members; the body touches the base's InventorySelectionState, which
// sits at +0x1C in the retail CTCInventoryBase (PDB +0x20 minus the 4-byte-narrower Components vector), so a local view is used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CTCInventoryBase_Retail {
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
    virtual void AddCloseInventoryEvent();                       // slot 32 (+0x80)
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;                       // +0x1C
    void SelectHighlightedItem(bool& selected);                  // 0x005BDD04
    void ProcessButtonAReleased();                               // 0x005BC65C
};
struct CTCInventoryWeapons_Retail : CTCInventoryBase_Retail {
    void ProcessButtonAReleased();
};
void CTCInventoryWeapons_Retail::ProcessButtonAReleased()
{
    if (InventorySelectionState == 1) {
        bool selected = false;
        SelectHighlightedItem(selected);
        if (selected)
            AddCloseInventoryEvent();
    }
    CTCInventoryBase_Retail::ProcessButtonAReleased();
}
