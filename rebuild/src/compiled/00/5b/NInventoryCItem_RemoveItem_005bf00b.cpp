// NInventory::CItem::RemoveItem(long count) -- take count items off the slot (never below zero); an emptied slot drops its per-item data and clears its type and average bought price
// Leaf: ?RemoveItem@CItem@NInventory@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BF00B, 49 bytes, thiscall `ret 4`; size-optimised TU (unaligned entry).
// Identity (proven): FableWin.pdb ?RemoveItem@CItem@NInventory@@QAEXJ@Z; Ego_r.pdb CItem {ItemType, NumberOfItems,
//   PPerItemData CCountedPointer<CPerItemData>, AverageBoughtPrice} (0x14 bytes) matches the +0 / +4 / +8 / +0x10 stores.
// Callees: CCountedPointer<NInventory::CPerItemData>::ReleaseData() 0x005C04BF (out of line; it clears Data and Info).
// Shape: CCountedPointer::Reset(0) inline (FableWin.pdb has ?Reset@?$CCountedPointer@VCPerItemData@NInventory@@@@QAEXPAV...):
//   ReleaseData, then the redundant `Data = 0` store retail keeps. The clamp is an inline helper taking the count by
//   reference (retail addresses it through `lea eax,[esi+4]` and re-tests with `cmp ebx,ebx`); written inline in the
//   body VC7.1 folds the test into the `sub` flags (`jns`). ClampToZero is a descriptive name (the helper is not recovered).
#pragma optimize("s", on)
class CThing { public:
    unsigned char  _pad00[0x8C];
    unsigned short DefGlobalIndex;                      // +0x8C (Ego_r.pdb +0x9C, minus 0x10 for the retail containers)
    long GetDefGlobalIndex() const { return DefGlobalIndex; }
};
class CCPPointerInfo;
namespace NInventory { class CPerItemData; }
template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    void ReleaseData();                                 // releases the count block and clears Data / Info
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = MakeInfo(data);
    }
    static CCPPointerInfo* MakeInfo(T* data);
};
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

inline long ClampToZero(long& value)
{
    if (value < 0)
        value = 0;
    return value;
}
void NInventory::CItem::RemoveItem(long count)
{
    NumberOfItems -= count;
    if (ClampToZero(NumberOfItems) == 0) {
        PPerItemData.Reset(0);
        AverageBoughtPrice = 0;
        ItemType = 0;
    }
}