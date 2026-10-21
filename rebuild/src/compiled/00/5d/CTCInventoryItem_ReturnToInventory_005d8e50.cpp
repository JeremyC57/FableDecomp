// CTCInventoryItem::ReturnToInventory() -- hands this item's thing back to the inventory it belongs to (PInventory's slot-83 virtual)
// Leaf: ReturnToInventory@CTCInventoryItem
// Retail: Fable.exe @ 0x005D8E50, 18 bytes. Catalog label NScript::CQ_AwakeningTheOracleScript::CChunkCollectionIndicator::OnPredicateFail
// is wrong (and so is the chunk-206 correction CTCShowableItem::OnShowEnded). Evidence: it sits between
// CTCInventoryItem::RemoveFromInventory 0x005D8E20 (same `[this+0x10]` PInventory / `[this+4]` thing pair, PInventory then
// zeroed) and the PInventory store 0x005D8E40; every caller (CTCInventory::OnItemSelected 0x00583E26,
// CTCDivineWrath::UnsheatheWeapon 0x0078E9C0, CEventShownItem dtor 0x008116D0, CTCCarriedActionUseRead 0x007E1EB0, ...)
// first fetches the thing's TC of interface 0x1a (CThing::Components lookup) and calls this on it; Ego_r.pdb lists
// ?ReturnToInventory@CTCInventoryItem@@QAEXXZ, the one no-argument non-virtual left without an address.
// Callee: CTCInventoryBase vtable 0x01253DBC slot 83 (+0x14c, 0x005BF4ED), taking the thing.
// Layout: engine/CTCInventoryItem.h (+0x0c PDef, +0x10 PInventory) over engine/CTCBase.h (+0x04 ParentThing); modelled
// locally with the virtual base so `this` stays a class (both headers are data-only).
class CThing;
class CDefPointeeBase;
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
    virtual void _v58();
    virtual void _v59();
    virtual void _v60();
    virtual void _v61();
    virtual void _v62();
    virtual void _v63();
    virtual void _v64();
    virtual void _v65();
    virtual void _v66();
    virtual void _v67();
    virtual void _v68();
    virtual void _v69();
    virtual void _v70();
    virtual void _v71();
    virtual void _v72();
    virtual void _v73();
    virtual void _v74();
    virtual void _v75();
    virtual void _v76();
    virtual void _v77();
    virtual void _v78();
    virtual void _v79();
    virtual void _v80();
    virtual void _v81();
    virtual void _v82();
    virtual void ReturnItemToInventory(CThing& thing);   // slot 83 (+0x14c)
};
}
class CTCBase {
public:
    virtual ~CTCBase();
    CThing*       ParentThing;                           // +0x04
    bool          ToBeRemoved;                           // +0x08
    bool          IsInGlobalUpdateTCs;                   // +0x09
};
class CTCInventoryItem : public CTCBase {
public:
    CDefPointeeBase*              PDef_Object;           // +0x0c CDefPointer<CInventoryItemDef const>
    NInventory::CTCInventoryBase* PInventory;            // +0x10
    void ReturnToInventory();
};

void CTCInventoryItem::ReturnToInventory()
{
    PInventory->ReturnItemToInventory(*ParentThing);
}