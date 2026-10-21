// CTCInventoryItem::GetInventoryType() const -- the item definition's InventoryType (the TC interface type the item stands for)
// Leaf: GetInventoryType@CTCInventoryItem
// Retail: Fable.exe @ 0x005D9030, 32 bytes. Catalog label NInventory::CCategory::GetCategoryIdentifier is wrong (ICF
// propagation): callers (CQuestManager::SetQuestAsCompleted 0x004B2077, CThingPlayerCreature::CreateAndAddObjectToInventory
// 0x006AC278, ...) fetch TC 0x1a from a thing first and use the result as a TC type (`cmp esi,112h` bound, then a
// UsingComponents bit test), matching Ego_r.pdb ?GetInventoryType@CTCInventoryItem@@QBE?AW4ETCInterfaceType@@XZ.
// Body: a CDefPointer copy of PDef (+0x0c; add-ref), read InventoryType (+0x50), release the copy.
// Layout: Ego_r.pdb CTCInventoryItem PDef +0x0c; CInventoryItemDef InventoryType +0x50 (the Ego_r.pdb offset; the
// callers' TC-type use of the value confirms the field).
// Shape: /O2 (`mov eax,[ecx+4] ; dec eax ; mov [ecx+4],eax`); the size-TU twin is 0x005BD1B3.
enum ETCInterfaceType { TC_INTERFACE_NONE };
struct CDefPointeeBase {
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                         // slot 1 (+4)
    unsigned long IVRefCount;                            // +0x04
    __forceinline void Release() { if (--IVRefCount == 0) Delete(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CInventoryItemDef : CDefPointeeBase {
    unsigned char    _pad08[0x48];                       // +0x08
    ETCInterfaceType InventoryType;                      // +0x50
};
class CTCInventoryItem {
public:
    unsigned char                  _pad00[0xc];          // +0x00 CTCBase
    CDefPointer<CInventoryItemDef> PDef;                 // +0x0c
    ETCInterfaceType GetInventoryType() const;
};
ETCInterfaceType CTCInventoryItem::GetInventoryType() const
{
    CDefPointer<CInventoryItemDef> def(PDef);
    return def->InventoryType;
}