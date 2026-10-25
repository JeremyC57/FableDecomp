// CTCInventoryQuests::GetInitialCategoryRepeatDelay() const -- 0.5f while the test-quest list mode is active, else the base inventory value
// (the inventory def +0x3C)
// Leaf: ?GetInitialCategoryRepeatDelay@CTCInventoryQuests@@UBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A0E0, 23 bytes (+9 int3); vtable 0x012585B4 (.?AVCTCInventoryQuests@@) slot 42.
// Name: Ego_r.exe ?GetInitialCategoryRepeatDelay@CTCInventoryQuests@@UBEMXZ 0x00597B30 then ?GetContinuedCategoryRepeatDelay@...
// 0x00597B50 (21 bytes each, this one 0x00597B30) = the retail pair 0x0061A0E0 / 0x0061A100 (slots 42 / 43); each calls the base
// getter of its own slot (NInventory::CTCInventoryBase::GetInitialCategoryRepeatDelay 0x005BD404, Ego_r.pdb UBEMXZ). Float constant @ 0x0122F59C = 0.5f.
// Shape: `mov al,[ecx+157h] ; test al,al ; je ; fld [const] ; ret ; call base ; ret` -- the base result is taken into a
// NAMED float local: a plain `return Base::f();` becomes a sibling-call `jmp` (GOTCHAS batch 5: a named float local
// blocks the tail jump). +0x157 is ActiveTestQuestMode (PDB +0x177). Local model (no engine header for CTCInventoryQuests).
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
    virtual float GetInitialCategoryRepeatDelay() const;    // slot 42 (0x005BD404)
    virtual float GetContinuedCategoryRepeatDelay() const;  // slot 43 (0x005BD427)
    virtual float GetInitialItemRepeatDelay() const;        // slot 44 (0x005BD44A)
    virtual float GetContinuedItemRepeatDelay() const;      // slot 45 (0x005BD46D)
    unsigned char _pad_0x04[0x130 - 4];                          // retail size 0x130
};

} // namespace NInventory

class CTCInventoryQuests : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryQuests@@, vtable 0x012585B4
public:
    unsigned char _pad_0x130[0x157 - 0x130];
    bool ActiveTestQuestMode;                                    // +0x157 (Ego_r.pdb +0x177)
    virtual float GetInitialCategoryRepeatDelay() const;         // slot 42 (0x0061A0E0)
    virtual float GetContinuedCategoryRepeatDelay() const;       // slot 43 (0x0061A100)
};

float CTCInventoryQuests::GetInitialCategoryRepeatDelay() const
{
    if (ActiveTestQuestMode)
        return 0.5f;
    float value = CTCInventoryBase::GetInitialCategoryRepeatDelay();
    return value;
}
