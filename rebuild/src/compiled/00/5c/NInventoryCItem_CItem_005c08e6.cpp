// NInventory::CItem::CItem(const CItem& other) -- copy an inventory slot: type, count, the shared per-item data (count bumped) and the average bought price
// Leaf: ??0CItem@NInventory@@QAE@ABV01@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C08E6, 43 bytes, thiscall `ret 4`, returns this; size-optimised TU.
// Identity (probable): the memberwise copy of a 0x14-byte Ego_r.pdb NInventory::CItem whose +8 member goes through
//   CCountedPointer<CPerItemData>'s copy constructor 0x005C05D5; its 7 callers are the vector<CItem> helpers around
//   0x005C0DEA..0x005C29AB (uninitialised copy / insert), its twin 0x005C0C24 the assignment. FableWin.pdb has
//   ??0CItem@NInventory@@QAE@ABV01@@Z (the compiler-generated copy).
// Callees: CCountedPointer<NInventory::CPerItemData>::CCountedPointer(const CCountedPointer&) 0x005C05D5.
// Shape: written out as the implicit memberwise copy, member order.
#pragma optimize("s", on)
#include "engine/CCountedPointer.h"
namespace NInventory { class CPerItemData; }
namespace NInventory {
class CItem {
public:
    long ItemType;                                      // +0x00
    long NumberOfItems;                                 // +0x04
    CCountedPointer<CPerItemData> PPerItemData;         // +0x08
    long AverageBoughtPrice;                            // +0x10
    CItem(const CItem& other);
    CItem& operator=(const CItem& other);
};
}

NInventory::CItem::CItem(const CItem& other)
    : ItemType(other.ItemType), NumberOfItems(other.NumberOfItems), PPerItemData(other.PPerItemData),
      AverageBoughtPrice(other.AverageBoughtPrice)
{
}