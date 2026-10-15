// CTCInventoryBase::ProcessInventoryInputUp() -- in category-selection state (0) moves to the previous category, in item-selection state (1) highlights the previous item, then fires the matching change hook
// Leaf: ProcessInventoryInputUp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDD8C, length 44 bytes; size TU. Vtable slot 38 (CTCInventoryClothing vtable
// 0x0124356C and the other inventory screens). The catalog label "ProcessInventoryInputDown" is propagated from the slot-39
// twin 0x005BDDB8; this body walks backwards (SelectPreviousCategory / HighlightPreviousItem), so it is the "up" handler.
// Callees: CTCInventoryBase::SelectPreviousCategory @ 0x005BDC08, CTCInventoryBase::HighlightPreviousItem @ 0x005BDCDC; virtual slots 28 (+0x70, category
// changed) and 29 (+0x74, highlight changed), both tail-called (`jmp [eax+70h]` / `jmp [eax+74h]`).
// Retail CTCInventoryBase: no engine header; InventorySelectionState (EInventorySelectionState) at +0x1C.
// Shape: `sub eax,0 ; je ; dec eax ; jne` is a two-case `switch` (case 1's block is laid out first).
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
    virtual void OnSelectedCategoryChanged();    // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged();     // slot 29 (+0x74)
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;       // +0x1C
    void SelectPreviousCategory();                                // 0x005BDC08
    void HighlightPreviousItem();                                 // 0x005BDCDC
    void ProcessInventoryInputUp();
};
void CTCInventoryBase_Retail::ProcessInventoryInputUp()
{
    switch (InventorySelectionState) {
    case 0:
        SelectPreviousCategory();
        OnSelectedCategoryChanged();
        break;
    case 1:
        HighlightPreviousItem();
        OnHighlightedItemChanged();
        break;
    }
}
