#pragma optimize("y", off)
#pragma optimize("s", on)
// CCategory::GetPerItemData(long) -- returns the per-item data object of the stack matching the item type (no end check)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BE8B1, length 34 bytes; size-optimised TU (`push 14h ; pop ecx` divisor, pass-through pushes).
// Callees: find_if<CItem*, SItemMatcher> @ 0x005C1921. Same shape as GetBoughtPrice @ 0x005BE839 but returns PPerItemData.Data (+0x08); the manifest label "GetBoughtPrice" is propagated.
// Retail CCategory (sizeof 0x2C): engine/CCategory.h is the Ego_r.pdb layout with a 16-byte
// vector<CItem>; the shipped vector is {first, last, end} (12 bytes), so every member after
// ItemsInCategory sits 4 bytes lower than the header says: +0x14 PDef (CDefPointer<const
// CInventoryCategoryDef>), +0x18 PInventory, +0x1C GlobalDefIndex, +0x20 SelectedItemIndex,
// +0x24 HighlightedItemIndex, +0x28 Selected, +0x29 DisplayItems. The header therefore cannot
// model `this` here; the local CCategory_Retail view below carries the PDB names at the retail offsets.
// Frame-pointer body in the size TU (`optimize("y",off)`): the matcher temporary lives at [ebp-8].
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
// Predicate object for the find_if instantiation: {item type, allow-multiple-slots flag}, an 8-byte POD with
// a user constructor and no copy constructor, so it is pushed dword by dword (the flag byte is homed on the
// stack and pushed as a dword, the type dword is forwarded from a register).
struct SItemMatcher {
    long m_itemType;                             // +0x00
    bool m_allowMultipleSlots;                   // +0x04
    SItemMatcher(long itemType, bool allowMultipleSlots) : m_itemType(itemType), m_allowMultipleSlots(allowMultipleSlots) { }
};
CItem_Methods* __fastcall FindMatchingItem(CItem_Methods* first, CItem_Methods* last, SItemMatcher matcher);   // 0x005C1921

struct CCategory_Retail {
    unsigned char PAssociatedThing[0x8];         // +0x00 CIntelligentPointer<CThing>
    CItemVector   ItemsInCategory;               // +0x08
    CPerItemData* GetPerItemData(long itemType);
};

CPerItemData* CCategory_Retail::GetPerItemData(long itemType)
{
    return FindMatchingItem(ItemsInCategory.first, ItemsInCategory.last, SItemMatcher(itemType, false))->PPerItemData_Data;
}