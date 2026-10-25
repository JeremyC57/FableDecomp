// NInventory::CTCInventoryBase::ProcessInventoryInputUp() -- in category-selection state (0) moves to the previous category, in item-selection state (1)
// highlights the previous item, then fires the matching change hook
// Leaf: ?ProcessInventoryInputUp@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDD8C, length 44 bytes; size TU. Vtable 0x01253DBC
// (.?AVCTCInventoryBase@NInventory@@) slot 38, inherited by the inventory screens.
// Name: Ego_r.exe ?ProcessInventoryInputUp@CTCInventoryBase@NInventory@@UAEXXZ 0x006206D3 precedes ...Down 0x00620703 (48 bytes
// each) as retail 0x005BDD8C precedes 0x005BDDB8 (44 bytes each); the Up body walks backwards. The class is namespaced (RTTI).
// Callees: CTCInventoryBase::SelectPreviousCategory @ 0x005BDC08, CTCInventoryBase::HighlightPreviousItem @ 0x005BDCDC (Ego_r.pdb
// ?SelectPreviousCategory@CTCInventoryBase@NInventory@@QAEXXZ / ?HighlightPreviousItem@...); virtual slots 28 (+0x70, category changed) and 29 (+0x74,
// highlight changed), both tail-called (`jmp [eax+70h]` / `jmp [eax+74h]`).
// Retail CTCInventoryBase: no engine header; InventorySelectionState (EInventorySelectionState) at +0x1C.
// Shape: `sub eax,0 ; je ; dec eax ; jne` is a two-case `switch` (case 1's block is laid out first).
#pragma optimize("s", on)
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void _v05();
    virtual void _v06();
    virtual void _v07();
    virtual void _v08();
    virtual void _v09();
    virtual void _v10();
    virtual void _v11();
    virtual void _v12();
    virtual void _v13();
    virtual void _v14();
    virtual void _v15();
    virtual void _v16();
    virtual void _v17();
    virtual void _v18();
    virtual void _v19();
    virtual void _v20();
    virtual void _v21();
    virtual void _v22();
    virtual void _v23();
    virtual void _v24();
    virtual void _v25();
    virtual void _v26();
    virtual void _v27();
    virtual void OnSelectedCategoryChanged();              // slot 28 (+0x70)
    virtual void OnHighlightedItemChanged();               // slot 29 (+0x74)
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void ProcessInventoryInputUp();                // slot 38 (+0x98)
    virtual void ProcessInventoryInputDown();              // slot 39 (+0x9c)
    unsigned char _pad_0x04[0x18];
    long          InventorySelectionState;       // +0x1C
    void SelectPreviousCategory();        // 0x005BDC08
    void HighlightPreviousItem();         // 0x005BDCDC
};

void CTCInventoryBase::ProcessInventoryInputUp()
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

} // namespace NInventory
