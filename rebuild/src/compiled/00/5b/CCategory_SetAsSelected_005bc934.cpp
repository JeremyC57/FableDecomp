#pragma optimize("s", on)
// CCategory::SetAsSelected(bool) -- records the selection state and, when becoming selected with a valid selected slot, tells the inventory (virtual slot 36) which item type was selected
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC934, length 59 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: CTCInventoryBase virtual slot 36 (+0x90) with the selected item's ItemType.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
#include "engine/CItem.h"

struct CItem_Methods : CItem {
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
    virtual void OnItemSelected(long itemType);   // slot 36 (+0x90)
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
    void SetAsSelected(bool selected);
};

void CCategory_Retail::SetAsSelected(bool selected)
{
    if (selected != Selected) {
        Selected = selected;
        if (selected) {
            if (!ItemsInCategory.empty()) {
                if (SelectedItemIndex > -1)
                    PInventory->OnItemSelected(ItemsInCategory[SelectedItemIndex].ItemType);
            }
        }
    }
}