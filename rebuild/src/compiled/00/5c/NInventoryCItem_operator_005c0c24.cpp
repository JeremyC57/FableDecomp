// NInventory::CItem::operator=(const CItem& other) -- assign an inventory slot: type, count, the shared per-item data (counted assignment) and the average bought price
// Leaf: ??4CItem@NInventory@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0C24, 43 bytes, thiscall `ret 4`, returns *this; size-optimised TU.
// Identity (probable): the memberwise assignment of a 0x14-byte Ego_r.pdb NInventory::CItem whose +8 member goes through
//   CCountedPointer<CPerItemData>::operator= 0x005C077F; callers are the vector<CItem> copy / erase helpers
//   (0x005C112F..0x005C2706). FableWin.pdb has ??4CItem@NInventory@@QAEAAV01@ABV01@@Z (the compiler-generated assignment).
// Callees: CCountedPointer<NInventory::CPerItemData>::operator=(const CCountedPointer&) 0x005C077F.
// Shape: written out as the implicit memberwise assignment, member order.
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

NInventory::CItem& NInventory::CItem::operator=(const CItem& other)
{
    ItemType = other.ItemType;
    NumberOfItems = other.NumberOfItems;
    PPerItemData = other.PPerItemData;
    AverageBoughtPrice = other.AverageBoughtPrice;
    return *this;
}