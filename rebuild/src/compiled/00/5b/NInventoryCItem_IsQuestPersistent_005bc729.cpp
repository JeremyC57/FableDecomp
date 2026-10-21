// NInventory::CItem::IsQuestPersistent() const -- true when the item has no per-item data, is flagged quest-persistent, or holds quest references
// Leaf: IsQuestPersistent@CItem_Methods
// Retail: Fable.exe @ 0x005BC729, 29 bytes; Ego_r.pdb ?IsQuestPersistent@CItem@NInventory@@QBE_NXZ. Null PPerItemData -> `mov al,1`;
// otherwise QuestPersistent (+0x18) || QuestPersistentRefCount (+0x1c) > 0 (`xor eax,eax ; inc eax` true arm). Only caller
// CCategory::GetAllNonEmptyInventoryItemsInCategory 0x005BF995.
// Layout: engine/CItem.h (PPerItemData Data +0x08). CPerItemData has no header; retail QuestPersistent/RefCount are at
// +0x18/+0x1c (Ego_r.pdb +0x1c/+0x20; the leading STLport vector<long> is 12 bytes, not 16).
// Caveat: the CPerItemData ctor 0x005C14A6 sets +0x18 = true and +0x1c = -1, so the Ego_r.pdb names on these two fields
// may not be the retail ones.
// Opt: size TU (`xor eax,eax ; inc eax` for the `||` true arm). Shape: the non-null test first puts the null arm last.
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
    bool IsQuestPersistent() const;
};
bool CItem_Methods::IsQuestPersistent() const
{
    if (PPerItemData_Data)
        return PPerItemData_Data->QuestPersistent || PPerItemData_Data->QuestPersistentRefCount > 0;
    return true;
}