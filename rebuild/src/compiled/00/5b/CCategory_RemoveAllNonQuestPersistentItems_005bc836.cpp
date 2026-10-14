#pragma optimize("s", on)
// CCategory::RemoveAllNonQuestPersistentItems() -- asks the owning inventory (virtual slot 79) to remove every non-empty stack that is not quest persistent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC836, length 91 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CItem::IsQuestPersistent @ 0x005BC746 (bool; manifest "CScriptBase::IsActiveThreadTerminating" is propagated), CTCInventoryBase virtual slot 79 (+0x13C) with (ItemType, NumberOfItems).
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// The element is re-indexed after the IsQuestPersistent call (retail reloads `first` and re-adds the
// stride) because the virtual call could reallocate the vector: the source indexes `ItemsInCategory[i]` at every use.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
    bool IsQuestPersistent();                 // 0x005BC746
};
// Retail std::vector<CItem> (12 bytes). size() is the signed element difference (`sub ; cdq ; idiv 14h`)
// converted to the unsigned size_type; elements are reached through `first`.
struct CItemVector {
    CItem_Methods* first;                        // +0x00
    CItem_Methods* last;                         // +0x04
    CItem_Methods* end_of_storage;               // +0x08
    unsigned long size() const { return (unsigned long)(last - first); }
    bool empty() const { return first == last; }
    CItem_Methods& operator[](unsigned long n) { return first[n]; }
};
struct CTCInventoryBase_Calls {
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
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void RemoveItems(long itemType, long numberOfItems);   // slot 79 (+0x13C)
};
struct CInventoryCategoryDef;

struct CCategory_Retail {
    unsigned char           PAssociatedThing[0x8];   // +0x00 CIntelligentPointer<CThing>
    CItemVector             ItemsInCategory;         // +0x08
    CInventoryCategoryDef*  PDef_Object;             // +0x14 CDefPointer<const CInventoryCategoryDef>::Object
    CTCInventoryBase_Calls* PInventory;              // +0x18
    long                    GlobalDefIndex;          // +0x1C
    long                    SelectedItemIndex;       // +0x20
    long                    HighlightedItemIndex;    // +0x24
    bool                    Selected;                // +0x28
    bool                    DisplayItems;            // +0x29
    void RemoveAllNonQuestPersistentItems();
};

void CCategory_Retail::RemoveAllNonQuestPersistentItems()
{
    for (unsigned long i = 0; i < ItemsInCategory.size(); ++i) {
        if (ItemsInCategory[i].NumberOfItems != 0 && !ItemsInCategory[i].IsQuestPersistent())
            PInventory->RemoveItems(ItemsInCategory[i].ItemType, ItemsInCategory[i].NumberOfItems);
    }
}