// NInventory::CItem::AddItem(CThing& thing, long price) -- add one of the thing to the slot: take its definition index as the item type, fold a known price (not -1) into the running average bought price, bump the count, then refresh the per-item stats from the thing
// Leaf: ?AddItem@CItem@NInventory@@QAEXAAVCThing@@J@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF556, 78 bytes, thiscall `ret 8`; size-optimised TU (unaligned entry).
// Identity (proven): FableWin.pdb ?AddItem@CItem@NInventory@@QAEXAAVCThing@@J@Z (the other overload takes three longs);
//   the reads match Ego_r.pdb CItem and CThing::DefGlobalIndex (unsigned short, retail +0x8C).
// Callees: NInventory::CItem::UpdateItemStats(const CThing&) 0x005BEDCB.
// Shape: an empty slot first forgets its average price (`and [ecx+10h],eax` with eax the zero count); the price fold
//   stores the weighted sum, the new count, then the quotient.
#pragma optimize("s", on)
class CThing { public:
    unsigned char  _pad00[0x8C];
    unsigned short DefGlobalIndex;                      // +0x8C (Ego_r.pdb +0x9C, minus 0x10 for the retail containers)
    long GetDefGlobalIndex() const { return DefGlobalIndex; }
};
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }
namespace NInventory {
class CItem {
public:
    long ItemType;                                      // +0x00
    long NumberOfItems;                                 // +0x04
    CCountedPointer<CPerItemData> PPerItemData;         // +0x08
    long AverageBoughtPrice;                            // +0x10
    void UpdateItemStats(const CThing& thing);          // 0x005BEDCB
    void RemoveItem(long count);
    void AddItem(CThing& thing, long price);
};
}

void NInventory::CItem::AddItem(CThing& thing, long price)
{
    if (NumberOfItems == 0)
        AverageBoughtPrice = 0;
    ItemType = thing.GetDefGlobalIndex();
    if (price != -1) {
        AverageBoughtPrice = AverageBoughtPrice * NumberOfItems + price;
        ++NumberOfItems;
        AverageBoughtPrice /= NumberOfItems;
    } else
        ++NumberOfItems;
    UpdateItemStats(thing);
}