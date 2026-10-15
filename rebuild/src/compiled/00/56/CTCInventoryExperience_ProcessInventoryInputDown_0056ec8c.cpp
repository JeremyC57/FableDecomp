// CTCInventoryExperience::ProcessInventoryInputDown() -- screen 0: base handling; spending: step the stat indicator down and refresh
// Retail 0x0056EC8C, 63 bytes (vtable 0x0124F108 slot 38; base slot 38 = CTCInventoryBase::ProcessInventoryInputDown 0x005BDD8C).
// Shape: a two-case `switch (State)` (+0x1C) lowered to `sub eax,0 ; je ; dec eax ; jne`.
//  case 0: tail-jump to the base handler.
//  case 1: DecrementStatIndicator 0x0056E5DE, then the base slider refresh 0x005BC58C (thiscall, three
//          arguments: &MenuDisplayListExperienceItem (+0x208), statIndicatorPos (+0x1BC),
//          statIndicatorMax (+0x1C0); retail's body there is an empty `ret 0Ch`), then the virtual
//          slot 29 refresh (`jmp [eax+74h]` = vfunc_29, 0x0056EF2D for this class).
// Retail offsets (the PDB header is 0x44..0x50 bytes wider ahead of these: 16-byte containers), local model.
#pragma optimize("s", on)

struct SLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
};

struct CTCInventoryBase {
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
    void ProcessInventoryInputDown();           // 0x005BDD8C
    void UpdateIndicatorSlider(SLongVector* items, long pos, long max);    // 0x005BC58C
    unsigned char _pad_004[0x18];
    long State;                                 // +0x01C
};

struct CTCInventoryExperience : CTCInventoryBase {
    unsigned char _pad_020[0x19C];
    long statIndicatorPos;                      // +0x1BC
    long statIndicatorMax;                      // +0x1C0
    unsigned char _pad_1c4[0x44];
    SLongVector MenuDisplayListExperienceItem;  // +0x208
    void DecrementStatIndicator();              // 0x0056E5DE
    void ProcessInventoryInputDown();
};

void CTCInventoryExperience::ProcessInventoryInputDown()
{
    switch (State) {
    case 0:
        CTCInventoryBase::ProcessInventoryInputDown();
        break;
    case 1:
        DecrementStatIndicator();
        UpdateIndicatorSlider(&MenuDisplayListExperienceItem, statIndicatorPos, statIndicatorMax);
        vfunc_29();
        break;
    }
}