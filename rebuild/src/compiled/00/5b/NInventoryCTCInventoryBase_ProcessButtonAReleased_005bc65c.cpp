#pragma optimize("s",on)
// NInventory::CTCInventoryBase::ProcessButtonAReleased() -- in category-selection state (0) releasing A enters item selection (state 1)
// Leaf: ?ProcessButtonAReleased@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe 0x005BC65C, 17 bytes (6 instructions); vtable 0x01253DBC (RTTI .?AVCTCInventoryBase@NInventory@@) slot 58.
// Name: Ego_r.exe lays out ProcessInventoryInputLeft 0x0061F4AC (17 bytes, 7 instructions), ProcessInventoryInputRight
// 0x0061F4BD and ProcessButtonAReleased 0x0061F4D0 (17 bytes, 6 instructions each, identical bodies), then
// ProcessButtonBReleased 0x0061F4E3 (30 bytes); retail has the same run at 0x005BC638 / 0x005BC649 / 0x005BC65C /
// 0x005BC66F, which are base vtable slots 40 / 41 / 58 / 59 (Ego_r here: 0x0061F4D0 (17 bytes, 6 instructions)). The BSim port gave all three
// 17-byte bodies "ProcessButtonAReleased". Overridden by CTCInventoryClothing 0x005B34D5, CTCInventoryTrade 0x0059B777, CTCInventoryWeapons 0x005C2E2A, CTCInventoryMap 0x00602F30.
// Callee slot 46 (+0xb8) is SetInventorySelectionState(EInventorySelectionState) (Ego_r.pdb
// ?SetInventorySelectionState@CTCInventoryBase@NInventory@@UAEXW4EInventorySelectionState@12@@Z).
// Retail bytes:  cmp dword ptr [ecx+1Ch],0 ; jne R ; mov eax,[ecx] ; push 1 ; call [eax+0B8h] ; R: ret
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
    virtual void ProcessInventoryInputLeft();           // slot 40 (+0xa0)
    virtual void ProcessInventoryInputRight();          // slot 41 (+0xa4)
    virtual void _v42();
    virtual void _v43();
    virtual void _v44();
    virtual void _v45();
    virtual void SetInventorySelectionState(EInventorySelectionState state);   // slot 46 (+0xb8)
    virtual void _v47();
    virtual void _v48();
    virtual void _v49();
    virtual void _v50();
    virtual void _v51();
    virtual void _v52();
    virtual void _v53();
    virtual void _v54();
    virtual void _v55();
    virtual void _v56();
    virtual void _v57();
    virtual void ProcessButtonAReleased();              // slot 58 (+0xe8)
    unsigned char            _pad_0x04[0x18];
    EInventorySelectionState InventorySelectionState;    // +0x1c
};

void CTCInventoryBase::ProcessButtonAReleased()
{
    if (InventorySelectionState == SELECTION_STATE_0)
        SetInventorySelectionState(SELECTION_STATE_1);
}

} // namespace NInventory
