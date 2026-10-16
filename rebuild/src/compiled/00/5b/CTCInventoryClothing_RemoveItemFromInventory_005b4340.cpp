// CTCInventoryClothing::RemoveItemFromInventory(long itemType, long count) -- removes items with the mannequin update suppressed; if a worn item went, selects the replacement for its slot and re-dresses the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B4340, length 98 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::IsItemSelected(long) @ 0x005BCA87, CTCInventoryBase::RemoveItemFromInventory(long, long)
// @ 0x005BF3C6 (manifest "GetItemDescription", propagated), the per-type replacement item @ 0x005D99B0 (__fastcall,
// manifest "GetInventoryCategory"), CTCInventoryBase::SelectItemWithDefIndex @ 0x005BDFB1,
// SetThingAsWearingSelectedClothes(CThing*) @ 0x005B406D.
// Shape: the old UseMannequin byte is kept in bl and restored at the end; the IsItemSelected result is a byte local
// ([ebp-1]); nothing happens after the base removal while the inventory is Dying (+0x88).
// Layout: retail CTCInventoryClothing ParentThing +0x04, CTCInventoryBase::Dying +0x88, UseMannequin +0x15C
// (engine/CTCInventoryClothing.h has it at +0x180: 16-byte PDB containers), so a local view is used.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;
long __fastcall GetReplacementItemForType(long itemType);      // 0x005D99B0
struct CTCInventoryBase_Retail {
    void*   __vftable;                                          // +0x000
    CThing* ParentThing;                                        // +0x004
    unsigned char _pad_0x008[0x80];
    bool    Dying;                                              // +0x088
    bool IsItemSelected(long itemType);                         // 0x005BCA87
    void RemoveItemFromInventory(long itemType, long count);    // 0x005BF3C6
    void SelectItemWithDefIndex(long defIndex);                 // 0x005BDFB1
};
struct CTCInventoryClothing_Retail : CTCInventoryBase_Retail {
    unsigned char _pad_0x08c[0x15C - 0x8C];                     // (base padded to 0x8C)
    bool    UseMannequin;                                       // +0x15C
    void SetThingAsWearingSelectedClothes(CThing* thing);       // 0x005B406D
    void RemoveItemFromInventory(long itemType, long count);
};
void CTCInventoryClothing_Retail::RemoveItemFromInventory(long itemType, long count)
{
    bool useMannequin = UseMannequin;
    UseMannequin = false;
    bool wasSelected = IsItemSelected(itemType);
    CTCInventoryBase_Retail::RemoveItemFromInventory(itemType, count);
    if (!Dying) {
        if (wasSelected)
            SelectItemWithDefIndex(GetReplacementItemForType(itemType));
        SetThingAsWearingSelectedClothes(ParentThing);
    }
    UseMannequin = useMannequin;
}