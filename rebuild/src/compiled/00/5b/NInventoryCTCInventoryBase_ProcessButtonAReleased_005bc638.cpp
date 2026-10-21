// NInventory::CTCInventoryBase::ProcessButtonAReleased() -- leaving the "item selected" selection state when the A button is released: state 1 drops back to state 0
// Leaf: ProcessButtonAReleased@CTCInventoryBase
// Label caveat: this is base vtable slot 40. Slots 50-61 are the A/B/X/Y Pressed/Held/Released handlers and slot 58
// (0x005BC65C, overridden by CTCInventoryTrade::ProcessButtonAReleased 0x0059B777) is ProcessButtonAReleased, so the
// catalog name kept here is probably wrong (a back/B-style handler); the bytes are unaffected.
// Retail: Fable.exe @ 0x005BC638, 17 bytes; vtable 0x01253DBC (RTTI CTCInventoryBase@NInventory). Derived overrides
// (0x0056F49C, 0x00581A4F, 0x0061A139) tail-jump here. Callee slot 46 (+0xb8) is the base's empty `ret 4`
// virtual taking one argument; Ego_r.pdb names it SetInventorySelectionState(EInventorySelectionState).
// Shape: a one-case switch (retail `dec eax ; jne`); an `== 1` test compiles to `cmp [ecx+0x1c],1`.
// Layout: retail CTCInventoryBase (no engine header): InventorySelectionState +0x1c (Ego_r.pdb +0x20, PDB minus 4 as for
// Categories +0x20 in landed 0x005BCE3C).
namespace NInventory {
class CTCInventoryBase {
public:
    enum EInventorySelectionState { SELECTION_STATE_0 = 0, SELECTION_STATE_1 = 1 };
    virtual ~CTCInventoryBase();                         // slot 0
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
    virtual void _v28();
    virtual void _v29();
    virtual void _v30();
    virtual void _v31();
    virtual void _v32();
    virtual void _v33();
    virtual void _v34();
    virtual void _v35();
    virtual void _v36();
    virtual void _v37();
    virtual void _v38();
    virtual void _v39();
    virtual void _v40();
    virtual void _v41();
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void SetInventorySelectionState(EInventorySelectionState state);   // slot 46 (+0xb8)
    virtual void ProcessButtonAReleased();
    unsigned char            _pad_0x04[0x18];
    EInventorySelectionState InventorySelectionState;    // +0x1c
};

void CTCInventoryBase::ProcessButtonAReleased()
{
    switch (InventorySelectionState) {
    case SELECTION_STATE_1:
        SetInventorySelectionState(SELECTION_STATE_0);
        break;
    }
}
}