// NInventory::CTCInventoryBase::GetItemOfTypeInInventory(long itemType) -- when the inventory holds at least one item of `itemType`, creates and returns a thing for it, else null
// Leaf: ?GetItemOfTypeInInventory@CTCInventoryBase@NInventory@@UAEPAVCThing@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF3F1, length 35 bytes; size TU (pass-through `push dword ptr [esp+8]`).
// Virtual slot 47 of the inventory screens (CTCInventoryClothing vtable 0x0124356C slot 47).
// Callees: CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, CTCInventoryBase::CreateInventoryObject(long) @ 0x005BE8F3.
// Retail CTCInventoryBase: only the two member calls are used (no data members touched).
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 47);
// Ego_r.exe ?GetItemOfTypeInInventory@CTCInventoryBase@NInventory@@UAEPAVCThing@@J@Z 0x006221AE is 35 bytes / 13 instructions like retail.
#pragma optimize("s", on)
class CThing;
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                                  // slot 0
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
    virtual CThing* GetItemOfTypeInInventory(long itemType);          // slot 47 (+0xBC)
    long    GetNumberOfItemsOfTypeInInventory(long itemType);   // 0x005BDF08
    CThing* CreateInventoryObject(long itemType);               // 0x005BE8F3
};

} // namespace NInventory
CThing* NInventory::CTCInventoryBase::GetItemOfTypeInInventory(long itemType)
{
    if (GetNumberOfItemsOfTypeInInventory(itemType) > 0)
        return CreateInventoryObject(itemType);
    return 0;
}