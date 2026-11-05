// Behaviour test for 0x005BD993 NInventory::CCategory::GetHighlightedItemMaxInCategory (plain C++, links the row object).
#include <stdio.h>
namespace NInventory {
class CItem { public:
    long ItemType; long NumberOfItems; void* Data; void* Info; long AverageBoughtPrice;   // 0x14 bytes
    long GetMaximumNumberOfItems() const;
};
class CCategory { public:
    unsigned char PAssociatedThing[8];
    CItem* First; CItem* Last; CItem* EndOfStorage;     // +0x08 vector<CItem>
    void* PDef; void* PInventory; long GlobalDefIndex; long SelectedItemIndex;
    long HighlightedItemIndex;                          // +0x24
    bool Selected; bool DisplayItems; unsigned char Pad[2];
    long GetHighlightedItemMaxInCategory() const;
};
}
static const NInventory::CItem* g_seen = 0;
static int g_calls = 0;
long NInventory::CItem::GetMaximumNumberOfItems() const { g_seen = this; ++g_calls; return 100 + ItemType; }

int main()
{
    NInventory::CItem items[4];
    for (int i = 0; i < 4; ++i) { items[i].ItemType = i; items[i].NumberOfItems = 0; }
    NInventory::CCategory cat;
    cat.First = items; cat.Last = items + 4; cat.EndOfStorage = items + 4;
    items[2].NumberOfItems = 3;
    cat.HighlightedItemIndex = 2;
    if (cat.GetHighlightedItemMaxInCategory() != 102 || g_calls != 1 || g_seen != &items[2]) { printf("FAIL 1\n"); return 1; }
    cat.HighlightedItemIndex = 1; g_calls = 0;
    if (cat.GetHighlightedItemMaxInCategory() != 0 || g_calls != 0) { printf("FAIL 2\n"); return 1; }
    items[3].NumberOfItems = 1; cat.HighlightedItemIndex = 3;
    if (cat.GetHighlightedItemMaxInCategory() != 103 || g_calls != 1 || g_seen != &items[3]) { printf("FAIL 3\n"); return 1; }
    printf("OK_0x005bd993\n");
    return 0;
}
