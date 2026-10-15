// CTCInventoryWeapons::GetTimeWieldedWeapon(long) -- collects the inventory items of a type and returns the first one's per-item "time wielded" value (0 when there is no item or no per-item data)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C99BE, 63 bytes.
// Callees: CTCInventoryBase::GetAllInventoryItemsOfType(long, vector<NInventory::CItem>*) @ 0x005BFA69, the
//          vector<NInventory::CItem> destructor @ 0x005C1E30 (manifest label "CActiveFile::OnReadFinished" is propagated).
//
// Shape: frame pointer in a size TU (`#pragma optimize("y", off)` + `optimize("s", on)`): the 12-byte vector local at
// [ebp-0Ch] zeroed through the result register (`xor esi,esi` is the result local, declared first, reused for the
// three stores), filled by the base call, `first != last` tested on the frame copies, the first item's
// PPerItemData object pointer (CItem +8, PDB NInventory::CItem) compared with that same zero register, then the
// vector's out-of-line destructor before `mov eax,esi`. The per-item data word at +0x10 is named from the
// function's own label. Local model: the retail Dinkumware vector is reconstructed by hand.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace NInventory {
struct CPerItemData {
    unsigned char _pad_0x00[0x10];
    long TimeWielded;                         // +0x10
};
struct CItem {
    long ItemType;                            // +0x00
    long NumberOfItems;                       // +0x04
    CPerItemData* PPerItemData;               // +0x08 CCountedPointer<CPerItemData>::m_pObject
    void* PPerItemDataBlock;                  // +0x0C
    long AverageBoughtPrice;                  // +0x10
};
}
struct CItemVector {                          // retail vector<NInventory::CItem>, 12 bytes
    NInventory::CItem* _First;
    NInventory::CItem* _Last;
    NInventory::CItem* _End;
    CItemVector() : _First(0), _Last(0), _End(0) {}
    ~CItemVector();                           // 0x005C1E30
    NInventory::CItem* begin() { return _First; }
    NInventory::CItem* end() { return _Last; }
};
struct CTCInventoryBase_Retail {
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    void GetAllInventoryItemsOfType(long itemType, CItemVector* items);   // 0x005BFA69
};
struct CTCInventoryWeapons : CTCInventoryBase_Retail {
    long GetTimeWieldedWeapon(long itemType);
};
long CTCInventoryWeapons::GetTimeWieldedWeapon(long itemType)
{
    long timeWielded = 0;
    CItemVector items;
    GetAllInventoryItemsOfType(itemType, &items);
    if (items.begin() != items.end()) {
        NInventory::CPerItemData* data = items.begin()->PPerItemData;
        if (data != 0)
            timeWielded = data->TimeWielded;
    }
    return timeWielded;
}