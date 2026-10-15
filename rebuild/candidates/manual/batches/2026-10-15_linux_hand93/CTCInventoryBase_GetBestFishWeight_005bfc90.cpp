// CTCInventoryBase::GetBestFishWeight(long fishType) -- returns the best recorded weight of the first stack of `fishType` (its per-item data's BestFishWeight), or 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BFC90, length 73 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on)
// (`and dword ptr [..],0` zero stores).
// Callees: CTCInventoryBase::GetAllInventoryItemsOfType(long, vector<CItem>&) @ 0x005BFA69, std::vector<NInventory::CItem>::~vector @ 0x005C1E30.
// Models: engine/CItem.h matches the retail CItem (0x14 bytes) but holds the counted pointer as an opaque Data/Info pair whose
// pointee type (NInventory::CPerItemData) has no header; the local CItem_Retail repeats the header's names with the pointee typed.
// Retail CPerItemData: the PDB layout with its leading std::vector<long> 12 bytes wide, so BestFishWeight sits at +0x20 (PDB +0x24).
// Shape: the float result is a named local; VC homes it in the dead `fishType` argument slot (`[ebp+8]`) across the vector
// destructor and copies the member with integer moves; the else arm is a literal `fldz`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CPerItemData_Retail {
    unsigned char Augmentations[0xC];                            // +0x00 std::vector<long> (retail 12 bytes)
    long          NumberOfKills;                                 // +0x0C
    long          TimeWielded;                                   // +0x10
    long          BestTrophyWitnessesAheadToDate;                // +0x14
    bool          QuestPersistent;                               // +0x18
    long          QuestPersistentRefCount;                       // +0x1C
    float         BestFishWeight;                                // +0x20
};
struct CItem_Retail {
    long                 ItemType;                               // +0x00
    long                 NumberOfItems;                          // +0x04
    CPerItemData_Retail* PPerItemData_Data;                      // +0x08 CCountedPointer<CPerItemData>::Data
    void*                PPerItemData_Info;                      // +0x0C
    long                 AverageBoughtPrice;                     // +0x10
};
struct CItemVector {
    CItem_Retail* first;
    CItem_Retail* last;
    CItem_Retail* end_of_storage;
    CItemVector() : first(0), last(0), end_of_storage(0) {}
    ~CItemVector();                                              // 0x005C1E30
};
struct CTCInventoryBase_Retail {
    void GetAllInventoryItemsOfType(long itemType, CItemVector& items);       // 0x005BFA69
    float GetBestFishWeight(long fishType);
};
float CTCInventoryBase_Retail::GetBestFishWeight(long fishType)
{
    CItemVector items;
    GetAllInventoryItemsOfType(fishType, items);
    float best;
    if (items.first != items.last && items.first->PPerItemData_Data != 0)
        best = items.first->PPerItemData_Data->BestFishWeight;
    else
        best = 0.0f;
    return best;
}
