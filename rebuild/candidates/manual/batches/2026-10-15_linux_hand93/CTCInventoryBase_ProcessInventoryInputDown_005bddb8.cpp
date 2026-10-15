// CTCInventoryBase::ProcessInventoryInputDown() -- in category-selection state (0) moves to the next category, in item-selection state (1) highlights the next item, then fires the matching change hook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDDB8, length 44 bytes; size TU. Vtable slot 39 (CTCInventoryClothing vtable
// 0x0124356C and the other inventory screens); slot 38 (0x005BDD8C) is the "up" twin.
// Callees: CTCInventoryBase::SelectNextCategory @ 0x005BDB94, CTCInventoryBase::HighlightNextItem @ 0x005BDCB4; virtual slots 28 (+0x70, category
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
    void SelectNextCategory();                                // 0x005BDB94
    void HighlightNextItem();                                 // 0x005BDCB4
    void ProcessInventoryInputDown();
};
void CTCInventoryBase_Retail::ProcessInventoryInputDown()
{
    switch (InventorySelectionState) {
    case 0:
        SelectNextCategory();
        OnSelectedCategoryChanged();
        break;
    case 1:
        HighlightNextItem();
        OnHighlightedItemChanged();
        break;
    }
}
