// NInventory::CTCInventoryBase::InitialActivate() -- consumes the items and selected-item names restored from a save: re-adds the items, re-selects each named item, then drops both serialised counted pointers
// Leaf: ?InitialActivate@CTCInventoryBase@NInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF9E3, length 97 bytes. Size TU: `#pragma optimize("s", on)` (`and dword ptr [m],0`).
// Callees: CTCInventoryBase::InitialiseInventoryWithItems(vector<CItem>*) @ 0x005BF8CB,
// CCountedPointer<vector<CItem>>::Release @ 0x005C0511 and CCountedPointer<vector<CCharString>>::Release @ 0x005C0563
// (manifest "_Dest_val<...COptimisedPrimitive>", byte twins), the definition-manager accessor @ 0x006869D0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CTCInventoryBase::SelectItemWithDefIndex @ 0x005BDFB1.
// Shape: each counted pointer is cleared with an inline `operator=(0)` (out-of-line release + one zero store of Data);
// the name vector is re-read through the counted pointer every iteration (the calls may touch it).
// Layout: retail CTCInventoryBase SerialisedItems +0x78, SerialisedSelectedItems +0x80 (CCountedPointer {Data, Info}).
// Name: the class is NInventory::CTCInventoryBase (RTTI vtable 0x01253DBC .?AVCTCInventoryBase@NInventory@@, slot 5);
// Ego_r.exe ?InitialActivate@CTCInventoryBase@NInventory@@UAEXXZ 0x00622A5A (CTCBase slot 5 = InitialActivate).
#pragma optimize("s", on)
struct CCharString { void* m_pData; };
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);     // 0x009AD410
};
namespace NInventory { struct CItem; }
struct CItemVector;
struct CCharStringVector { CCharString* first; CCharString* last; CCharString* end_of_storage; };
struct CSerialisedItemsPointer {                                  // CCountedPointer<vector<NInventory::CItem>>
    CItemVector* Data;
    void* Info;
    void Release();                                               // 0x005C0511
    void operator=(int) { Release(); Data = 0; }
};
struct CSerialisedSelectedItemsPointer {                          // CCountedPointer<vector<CCharString>>
    CCharStringVector* Data;
    void* Info;
    void Release();                                               // 0x005C0563
    void operator=(int) { Release(); Data = 0; }
};
namespace NInventory {

class CTCInventoryBase {
public:
    virtual ~CTCInventoryBase();                                  // slot 0
    virtual void _v01();
    virtual void _v02();
    virtual void _v03();
    virtual void _v04();
    virtual void InitialActivate();                                   // slot 5 (+0x14)
    unsigned char _pad_0x04[0x74];
    CSerialisedItemsPointer SerialisedItems;                      // +0x78
    CSerialisedSelectedItemsPointer SerialisedSelectedItems;      // +0x80
    CDefinitionManager* GetDefinitionManager();                   // 0x006869D0
    void InitialiseInventoryWithItems(CItemVector* items);        // 0x005BF8CB
    void SelectItemWithDefIndex(long defIndex);                   // 0x005BDFB1
};

} // namespace NInventory
void NInventory::CTCInventoryBase::InitialActivate()
{
    if (SerialisedItems.Data) {
        InitialiseInventoryWithItems(SerialisedItems.Data);
        SerialisedItems = 0;
    }
    if (SerialisedSelectedItems.Data) {
        for (CCharString* it = SerialisedSelectedItems.Data->first; it != SerialisedSelectedItems.Data->last; ++it) {
            long index = GetDefinitionManager()->GetDefGlobalIndexFromName(*it);
            if (index > 0)
                SelectItemWithDefIndex(index);
        }
        SerialisedSelectedItems = 0;
    }
}