// behaviour test for 0x005BC70A
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

int main()
{
    CItem_Methods it; memset(&it, 0, sizeof(it)); CPerItemData d; memset(&d, 0, sizeof(d));
    it.UpdateNonQuestPersistentRefCount(true);                       // null data: no crash
    it.PPerItemData_Data = &d;
    it.UpdateNonQuestPersistentRefCount(true); it.UpdateNonQuestPersistentRefCount(true);
    int ok = d.QuestPersistentRefCount == 2;
    it.UpdateNonQuestPersistentRefCount(false); ok &= d.QuestPersistentRefCount == 0;
    d.QuestPersistent = true; d.QuestPersistentRefCount = 5;
    it.UpdateNonQuestPersistentRefCount(true); it.UpdateNonQuestPersistentRefCount(false); ok &= d.QuestPersistentRefCount == 5;
    printf(ok ? "005bc70a_TEST PASS\n" : "005bc70a_TEST FAIL\n");
    return ok ? 0 : 1;
}