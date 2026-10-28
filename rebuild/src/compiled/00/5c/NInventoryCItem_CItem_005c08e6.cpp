// NInventory::CItem::CItem(const CItem& other) -- copy an inventory slot: type, count, the shared per-item data (count bumped) and the average bought price
// Leaf: ??0CItem@NInventory@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C08E6, 43 bytes, thiscall `ret 4`, returns this; size-optimised TU.
// Identity (probable): the memberwise copy of a 0x14-byte Ego_r.pdb NInventory::CItem whose +8 member goes through
//   CCountedPointer<CPerItemData>'s copy constructor 0x005C05D5; its 7 callers are the vector<CItem> helpers around
//   0x005C0DEA..0x005C29AB (uninitialised copy / insert), its twin 0x005C0C24 the assignment. FableWin.pdb has
//   ??0CItem@NInventory@@QAE@ABV01@@Z (the compiler-generated copy).
// Callees: CCountedPointer<NInventory::CPerItemData>::CCountedPointer(const CCountedPointer&) 0x005C05D5.
// Shape: written out as the implicit memberwise copy, member order.
#pragma optimize("s", on)
class CCPPointerInfo;
namespace NInventory { class CPerItemData; }
template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other);      // 0x005C05D5
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x005C077F
};
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