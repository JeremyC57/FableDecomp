// CTCInventoryTrade::ProcessButtonAReleased() -- in the item-selected state, performs the trade action, then runs the base A-release handling
// Leaf: ProcessButtonAReleased@CTCInventoryTrade
// Retail: Fable.exe @ 0x0059B777, 25 bytes; Ego_r.pdb ?ProcessButtonAReleased@CTCInventoryTrade@@UAEXXZ; CTCInventoryTrade vtable
// 0x01253034 slot 58 (the base slot 58 is 0x005BC65C, which this tail-jumps to). Base slots 50-61 are the A/B/X/Y
// Pressed/Held/Released handlers, so slot 58 is ProcessButtonAReleased. The own call is slot 68 (+0x110; trade 0x005AEBFE, base
// an empty stub), taken to be ProcessTradeAction (declared by the base and overridden by Trade in Ego_r.pdb).
// Layout: InventorySelectionState +0x1c as in 0x005BC638 (Ego_r.pdb +0x20).
namespace NInventory {
class CTCInventoryBase {
public:
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
    virtual void _v46();
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
    virtual void ProcessButtonAReleased();               // slot 58 (+0xe8)
    virtual void _v59();
    virtual void _v60();
    virtual void _v61();
    virtual void _v62();
    virtual void _v63();
    virtual void _v64();
    virtual void _v65();
    virtual void _v66();
    virtual void _v67();
    virtual void ProcessTradeAction();                   // slot 68 (+0x110)
    unsigned char _pad04[0x1c - 4];
    long          InventorySelectionState;               // +0x1c
};
}
class CTCInventoryTrade : public NInventory::CTCInventoryBase {
public:
    virtual void ProcessButtonAReleased();
};
void CTCInventoryTrade::ProcessButtonAReleased()
{
    if (InventorySelectionState == 1)
        ProcessTradeAction();
    NInventory::CTCInventoryBase::ProcessButtonAReleased();
}