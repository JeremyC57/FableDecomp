// CTCInventoryExperience::ProcessInventoryInputUp() -- screen 0: base handling; spending: step the stat indicator down (the previous value) and refresh
// Leaf: ?ProcessInventoryInputUp@CTCInventoryExperience@@UAEXXZ
// Retail 0x0056EC8C, 63 bytes (vtable 0x0124F11C slot 38; base slot 38 = NInventory::CTCInventoryBase::ProcessInventoryInputUp
// 0x005BDD8C).
// Shape: a two-case `switch (State)` (+0x1C) lowered to `sub eax,0 ; je ; dec eax ; jne`.
//  case 0: tail-jump to the base handler.
//  case 1: DecrementStatIndicator 0x0056E5DE, then the base slider refresh 0x005BC58C (thiscall, three
//          arguments: &MenuDisplayListExperienceItem (+0x208), statIndicatorPos (+0x1BC),
//          statIndicatorMax (+0x1C0); retail's body there is an empty `ret 0Ch`), then the virtual
//          slot 29 refresh (`jmp [eax+74h]` = vfunc_29, 0x0056EF2D for this class).
// Name: base vtable slots 38 / 39 are NInventory::CTCInventoryBase::ProcessInventoryInputUp 0x005BDD8C (previous category /
// item) and ...Down 0x005BDDB8 (next); Ego_r.exe defines every class's Up before its Down (base 0x006206D3 / 0x00620703,
// CTCInventoryExperience 0x005E45D8 (64 bytes) / 0x005E4618 (62 bytes) against retail 0x0056EC8C (63) / 0x0056ECCB (61)) and retail keeps that order with Up in slot 38 in every inventory
// vtable; each override tail-jumps to the base handler of its own slot. The earlier Up/Down names of this pair were swapped.
// Retail offsets (the PDB header is 0x44..0x50 bytes wider ahead of these: 16-byte containers), local model.
#pragma optimize("s", on)

struct SLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};

namespace NInventory {

class CTCInventoryBase {
public:
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
    virtual void vfunc_29();                    // slot 29 (+0x74)
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void ProcessInventoryInputUp();           // slot 38 (0x005BDD8C)
    virtual void ProcessInventoryInputDown();         // slot 39 (0x005BDDB8)
    void UpdateIndicatorSlider(SLongVector* items, long pos, long max);    // 0x005BC58C
    unsigned char _pad_004[0x18];
    long State;                                 // +0x01C (InventorySelectionState)
};

} // namespace NInventory

class CTCInventoryExperience : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryExperience@@, vtable 0x0124F11C
public:
    unsigned char _pad_020[0x19C];
    long statIndicatorPos;                      // +0x1BC
    long statIndicatorMax;                      // +0x1C0
    unsigned char _pad_1c4[0x44];
    SLongVector MenuDisplayListExperienceItem;  // +0x208
    void DecrementStatIndicator();              // 0x0056E5DE
    virtual void ProcessInventoryInputUp();     // slot 38 (0x0056EC8C)
    virtual void ProcessInventoryInputDown();   // slot 39 (0x0056ECCB)
};

void CTCInventoryExperience::ProcessInventoryInputUp()
{
    switch (State) {
    case 0:
        CTCInventoryBase::ProcessInventoryInputUp();
        break;
    case 1:
        DecrementStatIndicator();
        UpdateIndicatorSlider(&MenuDisplayListExperienceItem, statIndicatorPos, statIndicatorMax);
        vfunc_29();
        break;
    }
}
