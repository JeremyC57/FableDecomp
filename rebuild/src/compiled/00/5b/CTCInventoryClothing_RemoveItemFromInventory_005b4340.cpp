// CTCInventoryClothing::RemoveItemFromInventory(long itemType, long count) -- removes items with the mannequin update suppressed; if a worn item went, selects the replacement for its slot and re-dresses the owner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B4340, length 98 bytes. Frame-pointer size TU: optimize("s", on).
// Callees: CTCInventoryBase::IsItemSelected(long) @ 0x005BCA87, CTCInventoryBase::RemoveItemFromInventory(long, long)
// @ 0x005BF3C6 (manifest "GetItemDescription", propagated), the per-type replacement item @ 0x005D99B0 (__fastcall,
// manifest "GetInventoryCategory"), CTCInventoryBase::SelectItemWithDefIndex @ 0x005BDFB1,
// SetThingAsWearingSelectedClothes(CThing*) @ 0x005B406D.
// Shape: the old UseMannequin byte is kept in bl and restored at the end; the IsItemSelected result is a byte local
// ([ebp-1]); nothing happens after the base removal while the inventory is Dying (+0x88).
// Layout: retail CTCInventoryClothing ParentThing +0x04, CTCInventoryBase::Dying +0x88, UseMannequin +0x15C
// (engine/CTCInventoryClothing.h has it at +0x180: 16-byte PDB containers), so a local view is used.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: this row's object is byte-identical without it (size mode keeps the
//   leaf's frame; T3 nf416 audit).
#pragma optimize("s", on)
class CThing;
// NF64 (round 7, q10647): the free stand-in `GetReplacementItemForType` is the static __fastcall CTCInventoryItem::GetItemToSelectUponRemoval (Ego_r.pdb / FableWin.pdb; the
//   NF59 self row defines it), declared by engine/CTCInventoryItem.h.
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_GETITEMTOSELECTUPONREMOVAL
#include "engine/CTCInventoryItem.h"
namespace NInventory {
class CTCInventoryBase { public:                                // vptr +0x000
    CThing* ParentThing;                                        // +0x004
    unsigned char _pad_0x008[0x80];
    bool    Dying;                                              // +0x088
    virtual void RemoveItemFromInventory(long itemType, long count);   // 0x005BF3C6 Ego_r.pdb ?RemoveItemFromInventory@CTCInventoryBase@NInventory@@UAEXJJ@Z
protected:
    bool IsItemSelected(long itemType) const;                   // 0x005BCA87 Ego_r.pdb ?IsItemSelected@CTCInventoryBase@NInventory@@IBE_NJ@Z
    void SelectItemWithDefIndex(long defIndex);                 // 0x005BDFB1 Ego_r.pdb ?SelectItemWithDefIndex@CTCInventoryBase@NInventory@@IAEXJ@Z
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase { public:
    unsigned char _pad_0x08c[0x15C - 0x8C];                     // (base padded to 0x8C)
    bool    UseMannequin;                                       // +0x15C
    virtual void RemoveItemFromInventory(long itemType, long count);   // Ego_r.pdb ?RemoveItemFromInventory@CTCInventoryClothing@@UAEXJJ@Z
private:
    void SetThingAsWearingSelectedClothes(CThing& thing);       // 0x005B406D Ego_r.pdb ?SetThingAsWearingSelectedClothes@CTCInventoryClothing@@AAEXAAVCThing@@@Z
};
void CTCInventoryClothing::RemoveItemFromInventory(long itemType, long count)
{
    bool useMannequin = UseMannequin;
    UseMannequin = false;
    bool wasSelected = IsItemSelected(itemType);
    NInventory::CTCInventoryBase::RemoveItemFromInventory(itemType, count);
    if (!Dying) {
        if (wasSelected)
            SelectItemWithDefIndex(CTCInventoryItem::GetItemToSelectUponRemoval(itemType));
        SetThingAsWearingSelectedClothes(*ParentThing);
    }
    UseMannequin = useMannequin;
}