// CTCInventoryBase::SetCanItemBeUsed(long) -- records whether the highlighted item (by def index) can be used: true when the index is positive and the item def lookup finds a record
// Leaf: SetCanItemBeUsed
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC4DF, length 34 bytes.
// Callees: the item-def lookup @ 0x005D9710 (`__fastcall`, the def index in ecx; returns the matching
//          inventory record or 0 -- manifest label "GetInventoryCategory").
// Callers: the CTCInventory highlight refresh @ 0x005819B0 calls 0x005BC505 then 0x005BC4DF with the same index.
// Layout: CTCInventoryBase has no engine header; from the Ego_r.pdb layout with the shipped 12-byte vectors
//         (Components +0x0C, Categories +0x20): HighlightedItemNumOwned +0x2C, HighlightedItemMaxOwned +0x30,
//         CanItemBeQuickAssigned +0x34, CanItemBeUsed +0x35.
// Pragma: optimize("s", on) -- size TU (one shared epilogue; the speed build duplicates it).
// Shape: a non-positive index stores false directly; otherwise the lookup's non-null test is stored
//        (`test eax,eax ; setne al`).

long __fastcall LookUpInventoryItemDef(long defIndex);   // 0x005D9710

struct CTCInventoryBase_Retail {
    unsigned char _pad_0x00[0x2C];
    long          HighlightedItemNumOwned;        // +0x2C
    long          HighlightedItemMaxOwned;        // +0x30
    bool          CanItemBeQuickAssigned;         // +0x34
    bool          CanItemBeUsed;                  // +0x35

    void SetCanItemBeUsed(long defIndex);
};

void CTCInventoryBase_Retail::SetCanItemBeUsed(long defIndex)
{
    if (defIndex > 0)
        CanItemBeUsed = LookUpInventoryItemDef(defIndex) != 0;
    else
        CanItemBeUsed = false;
}
