// CItem::GetInventoryItemDef() const -- the counted CInventoryItemDef of a non-empty slot: the item type's def from the global definition manager, narrowed with GetSubDef; a null pointer for an empty slot
// Grade: DIFFER(126v114) -- the lane hoists one zero register (`xor esi,esi`) for the null tests and frame zeroing where retail uses `and [ebp-4],0`; the base def lands at [ebp-8] (retail [ebp-4]) and is not cached in esi; the sub-def copy-out re-reads its frame slot instead of the merged `inc ; dec` through ecx
// Leaf: GetInventoryItemDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BD82E, length 114 bytes.
// Callees: CDefinitionManager::GetDef<CDefClassBase>(long, CDefPointer&) @ 0x0042AF3C (the manager at
// g_pMainGameComponent(0x013B86A0) +0x24 -> +0x10), CDefClassBase::GetSubDef<CInventoryItemDef>(CDefPointer&) @ 0x005BADE1
// (both landed); the defs' virtual slot 1 (Delete) on the last release.
// Model: engine/CItem.h (ItemType +0, NumberOfItems +4); the counted defs as the landed CItem::GetMaximumNumberOfItems
// 0x005BD931 (vptr, IVRefCount +4). Pragmas: the frame-pointer size TU of the CItem module (`optimize("y", off)` +
// `("s", on)`).
// Shape: both def pointers are zeroed frame words (`and [ebp-4],0`); `CDefPtr sub; ...; return sub;` copies out with the
// merged `inc ; dec ; jne` (VC7.1 has no NRVO); the base def is read once into esi and released by hand after the copy.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CInventoryItemDef : CIVCountedPointeeBase {
};
struct CInventoryItemDefPointer {                  // CDefPointer<CInventoryItemDef const>
    CInventoryItemDef* Object;
    __forceinline CInventoryItemDefPointer() : Object(0) {}
    __forceinline CInventoryItemDefPointer(const CInventoryItemDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
    __forceinline ~CInventoryItemDefPointer()
    {
        if (Object && --Object->IVRefCount == 0)
            Object->Delete();
    }
};
struct CDefClassBase : CIVCountedPointeeBase {
    void GetSubDef(CInventoryItemDefPointer& out); // 0x005BADE1
};
struct CDefClassBasePointer {                      // CDefPointer<CDefClassBase const>
    CDefClassBase* Object;
    __forceinline CDefClassBasePointer() : Object(0) {}
    __forceinline ~CDefClassBasePointer()
    {
        CDefClassBase* def = Object;
        if (def && --def->IVRefCount == 0)
            def->Delete();
    }
    CDefClassBase* operator->() const { return Object; }
};
struct CDefinitionManager {
    bool GetDef(long index, CDefClassBasePointer& out);   // 0x0042AF3C
};
struct CGameComponentDefs {
    unsigned char       _pad00[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10
};
struct CMainGameComponent {
    unsigned char       _pad00[0x24];
    CGameComponentDefs* Defs;                      // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
struct CItem_Methods : CItem {
    CInventoryItemDefPointer GetInventoryItemDef() const;
};

CInventoryItemDefPointer CItem_Methods::GetInventoryItemDef() const
{
    if (NumberOfItems > 0) {
        CDefClassBasePointer def;
        g_pMainGameComponent->Defs->DefinitionManager->GetDef(ItemType, def);
        CInventoryItemDefPointer sub;
        def->GetSubDef(sub);
        return sub;
    }
    return CInventoryItemDefPointer();
}
