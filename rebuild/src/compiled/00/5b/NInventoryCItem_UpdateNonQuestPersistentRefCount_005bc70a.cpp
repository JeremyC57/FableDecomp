// NInventory::CItem::UpdateNonQuestPersistentRefCount(bool increment) -- bumps (or clears) the per-item quest reference count unless the item is flagged quest-persistent
// Leaf: UpdateNonQuestPersistentRefCount@CItem_Methods
// Retail: Fable.exe @ 0x005BC70A, 31 bytes, `ret 4`. Body: with PPerItemData non-null and QuestPersistent (+0x18) clear,
// `++QuestPersistentRefCount` (+0x1c) when the argument is true, else set it to 0. Only caller 0x005BC817.
// Layout: engine/CItem.h (PPerItemData Data +0x08); CPerItemData retail offsets as in 0x005BC729 / ctor 0x005C14A6.
// Opt: size TU (one `xor ecx,ecx` zero shared by the byte compares and the store). Shape: the pointer read into a
// local frees ecx for the zero (reading the member twice allocates edx).
#pragma optimize("s", on)
#include "engine/CItem.h"
struct CPerItemData {
    unsigned char Augmentations[0xc];                    // +0x00 STLport vector<long>
    long          NumberOfKills;                         // +0x0c
    long          TimeWielded;                           // +0x10
    long          BestTrophyWitnessesAheadToDate;        // +0x14
    bool          QuestPersistent;                       // +0x18
    long          QuestPersistentRefCount;               // +0x1c
    float         BestFishWeight;                        // +0x20
};
struct CItem_Methods : CItem {
    void UpdateNonQuestPersistentRefCount(bool increment);
};
void CItem_Methods::UpdateNonQuestPersistentRefCount(bool increment)
{
    CPerItemData* data = PPerItemData_Data;
    if (data && !data->QuestPersistent) {
        if (increment)
            ++data->QuestPersistentRefCount;
        else
            data->QuestPersistentRefCount = 0;
    }
}