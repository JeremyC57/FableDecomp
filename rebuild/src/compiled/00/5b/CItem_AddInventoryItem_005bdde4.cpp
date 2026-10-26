// CItem::AddInventoryItem(const CItem& other) -- merge another stack into this slot: an empty slot takes the other item wholesale; otherwise add as many as still fit and re-average the bought price over the new count
// Leaf: AddInventoryItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BDDE4, length 108 bytes.
// Callees: CCountedPointer<NInventory::CPerItemData>::operator=(const CCountedPointer&) @ 0x005C077F,
// CItem::GetMaximumNumberOfItems @ 0x005BD931 (landed).
// Model: engine/CItem.h (ItemType +0, NumberOfItems +4, PPerItemData +8 (Data/Info), AverageBoughtPrice +0x10).
// Pragmas: the CItem module's frame-pointer size TU.
// Shape: the room left is a frame local so that `std::min` can return a reference to it (`cmp [ecx],eax ; mov eax,ecx ;
// jl ; lea eax,[ebp-4]`); the new count is stored before the `idiv` that averages the price.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPerItemData;
class CCPPointerInfo;
struct CPerItemDataPointer {                       // CCountedPointer<NInventory::CPerItemData>
    CPerItemData*   Data;                          // +0x00
    CCPPointerInfo* Info;                          // +0x04
    CPerItemDataPointer& operator=(const CPerItemDataPointer& other);   // 0x005C077F
};
template <class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CItem {
    long                ItemType;                  // +0x00
    long                NumberOfItems;             // +0x04
    CPerItemDataPointer PPerItemData;              // +0x08
    long                AverageBoughtPrice;        // +0x10
    long GetMaximumNumberOfItems();                // 0x005BD931
    void AddInventoryItem(const CItem& other);
};

void CItem::AddInventoryItem(const CItem& other)
{
    if (NumberOfItems == 0) {
        NumberOfItems = other.NumberOfItems;
        ItemType = other.ItemType;
        PPerItemData = other.PPerItemData;
        AverageBoughtPrice = other.AverageBoughtPrice;
    } else {
        long room = GetMaximumNumberOfItems() - NumberOfItems;
        long added = Min(room, other.NumberOfItems);
        long total = other.AverageBoughtPrice * added + AverageBoughtPrice * NumberOfItems;
        NumberOfItems = added + NumberOfItems;
        AverageBoughtPrice = total / NumberOfItems;
    }
}