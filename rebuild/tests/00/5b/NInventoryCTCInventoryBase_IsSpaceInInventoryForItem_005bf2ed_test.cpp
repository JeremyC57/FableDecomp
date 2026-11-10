// Behaviour test for NInventory::CTCInventoryBase::IsSpaceInInventoryForItem (0x005BF2ED): the item's inventory category picks the
// category from Categories, and that category answers for the item.
#include <stdio.h>
#if !defined(_MSC_VER) || defined(_WIN64)
#define __fastcall
#endif
static long g_lastAsked;
class CTCInventoryItem { public:
    static long __fastcall GetInventoryCategory(long itemDefIndex) { return itemDefIndex / 100; }
};
namespace NInventory {
class CCategory { public:
    long Index; long Space; unsigned char _body[0x2C - 8];
    bool IsSpaceInCategoryForItem(long itemDefIndex) const { g_lastAsked = itemDefIndex; return Space > 0; }
};
CCategory* __fastcall FindCategoryWithDefIndex(CCategory* first, CCategory* last, long categoryDefIndex)
{
    for (; first != last; ++first)
        if (first->Index == categoryDefIndex)
            break;
    return first;
}
struct CCategoryVector { CCategory* _M_start; CCategory* _M_finish; CCategory* _M_end_of_storage; };
class CTCInventoryBase { public:
    unsigned char _pad00[0x20];
    CCategoryVector Categories;
    bool IsSpaceInInventoryForItem(long itemDefIndex) const
    {
        return FindCategoryWithDefIndex(Categories._M_start, Categories._M_finish,
                                        CTCInventoryItem::GetInventoryCategory(itemDefIndex))->IsSpaceInCategoryForItem(itemDefIndex);
    }
};
}
int main()
{
    NInventory::CCategory cats[3];
    cats[0].Index = 1; cats[0].Space = 0;
    cats[1].Index = 2; cats[1].Space = 3;
    cats[2].Index = 3; cats[2].Space = 0;
    NInventory::CTCInventoryBase inv;
    inv.Categories._M_start = cats; inv.Categories._M_finish = cats + 3; inv.Categories._M_end_of_storage = cats + 3;
    if (!inv.IsSpaceInInventoryForItem(205) || g_lastAsked != 205) { printf("005bf2ed_TEST FAIL\n"); return 1; }
    if (inv.IsSpaceInInventoryForItem(117) || g_lastAsked != 117) { printf("005bf2ed_TEST FAIL\n"); return 1; }
    if (inv.IsSpaceInInventoryForItem(399)) { printf("005bf2ed_TEST FAIL\n"); return 1; }
    printf("005bf2ed_TEST PASS\n");
    return 0;
}
