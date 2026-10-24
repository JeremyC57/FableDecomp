// behaviour test for 005c135e: STLport random-access __copy_backward over 0x14-byte inventory items
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
struct random_access_iterator_tag {};
static int g_assigns = 0;
struct CItem {
    int ItemType;
    int NumberOfItems;
    int PPerItemData[2];
    int AverageBoughtPrice;
    CItem& operator=(const CItem& other)
    {
        ItemType = other.ItemType; NumberOfItems = other.NumberOfItems;
        PPerItemData[0] = other.PPerItemData[0]; PPerItemData[1] = other.PPerItemData[1];
        AverageBoughtPrice = other.AverageBoughtPrice;
        ++g_assigns;
        return *this;
    }
};
template <class RandomAccessIter, class BidirectionalIter, class Distance>
BidirectionalIter __fastcall CopyBackwardModel(RandomAccessIter first, RandomAccessIter last, BidirectionalIter result,
                                               const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
int main()
{
    CItem a[10];
    for (int i = 0; i < 10; ++i) { a[i].ItemType = i; a[i].NumberOfItems = 100 + i; a[i].PPerItemData[0] = a[i].PPerItemData[1] = i * 3; a[i].AverageBoughtPrice = 7 * i; }
    random_access_iterator_tag tag;
    int bad = 0;
    /* overlapping shift right by two: [0,6) -> [2,8) */
    CItem* r = CopyBackwardModel(a, a + 6, a + 8, tag, (int*)0);
    if (r != a + 2 || g_assigns != 6) ++bad;
    for (int i = 0; i < 10; ++i) {
        int want = (i < 2) ? i : (i < 8 ? i - 2 : i);
        if (a[i].ItemType != want || a[i].NumberOfItems != 100 + want || a[i].AverageBoughtPrice != 7 * want) ++bad;
    }
    /* empty range: nothing assigned, result returned unchanged */
    g_assigns = 0;
    if (CopyBackwardModel(a + 3, a + 3, a + 9, tag, (int*)0) != a + 9 || g_assigns != 0) ++bad;
    /* reversed range (last < first): no trip */
    if (CopyBackwardModel(a + 5, a + 3, a + 9, tag, (int*)0) != a + 9 || g_assigns != 0) ++bad;
    if (bad) { std::printf("005c135e_TEST FAIL\n"); return 1; }
    std::printf("005c135e_TEST PASS\n");
    return 0;
}