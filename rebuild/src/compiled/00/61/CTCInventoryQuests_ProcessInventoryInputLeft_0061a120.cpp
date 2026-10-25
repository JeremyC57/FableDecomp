// CTCInventoryQuests::ProcessInventoryInputLeft() -- outside the test-quest list mode, with at least one category, the base inventory's
// ProcessInventoryInputLeft handling runs
// Leaf: ?ProcessInventoryInputLeft@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A120, 32 bytes; vtable 0x012585B4 (RTTI .?AVCTCInventoryQuests@@) slot 40.
// Name: Ego_r.exe has ?ProcessInventoryInputLeft@CTCInventoryQuests@@UAEXXZ 0x00597B70 then ...Right 0x00597B90 (31 bytes each,
// identical bodies), the retail pair 0x0061A120 / 0x0061A140 (slots 40 / 41), each tail-jumping to the base handler of its
// own slot (0x005BC638 = NInventory::CTCInventoryBase::ProcessInventoryInputLeft, 0x005BC649 = ...Right).
// Callees: NInventory::CTCInventoryBase::GetCategoryCount @ 0x005BCE2F (`(Categories.last - first) / 0x2C`), the base
// ProcessInventoryInputLeft @ 0x005BC638 (qualified, a void tail jump).
// Shape: `mov al,[esi+157h] ; test ; jne out ; call count ; test eax,eax ; jle out ; mov ecx,esi ; pop esi ; jmp base`
// -- an `&&` guard with the qualified base call as a void tail call. +0x157 is ActiveTestQuestMode (PDB +0x177).
// Local model: no engine header for CTCInventoryQuests / CTCInventoryBase.
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                          // slot 0
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
    virtual void ProcessInventoryInputLeft();             // slot 40 (0x005BC638)
    virtual void ProcessInventoryInputRight();            // slot 41 (0x005BC649)
    unsigned char _pad_0x04[0x130 - 4];
    long GetCategoryCount() const;                      // 0x005BCE2F
};

} // namespace NInventory

class CTCInventoryQuests : public NInventory::CTCInventoryBase {   // RTTI .?AVCTCInventoryQuests@@, vtable 0x012585B4
public:
    virtual void ProcessInventoryInputLeft();           // slot 40 (0x0061A120)
    virtual void ProcessInventoryInputRight();          // slot 41 (0x0061A140)
    unsigned char _pad_0x130[0x157 - 0x130];
    bool          ActiveTestQuestMode;                  // +0x157 (Ego_r.pdb +0x177)
};

void CTCInventoryQuests::ProcessInventoryInputLeft()
{
    if (!ActiveTestQuestMode && GetCategoryCount() > 0)
        CTCInventoryBase::ProcessInventoryInputLeft();
}
