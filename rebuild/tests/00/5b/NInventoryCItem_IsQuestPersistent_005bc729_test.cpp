// behaviour test for 0x005BC729
#include <stdio.h>
#include <string.h>
struct CPerItemData;
struct CCPPointerInfo;
struct CItem { long ItemType; long NumberOfItems; CPerItemData* PPerItemData_Data; CCPPointerInfo* PPerItemData_Info; long AverageBoughtPrice; };
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

int main()
{
    CItem_Methods it; memset(&it, 0, sizeof(it)); CPerItemData d; memset(&d, 0, sizeof(d));
    int ok = it.IsQuestPersistent() == true;
    it.PPerItemData_Data = &d; ok &= it.IsQuestPersistent() == false;
    d.QuestPersistent = true; ok &= it.IsQuestPersistent() == true;
    d.QuestPersistent = false; d.QuestPersistentRefCount = 2; ok &= it.IsQuestPersistent() == true;
    d.QuestPersistentRefCount = -1; ok &= it.IsQuestPersistent() == false;
    printf(ok ? "005bc729_TEST PASS\n" : "005bc729_TEST FAIL\n");
    return ok ? 0 : 1;
}