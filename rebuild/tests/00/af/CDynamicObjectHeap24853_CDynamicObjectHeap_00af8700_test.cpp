// behaviour test for 0x00AF8700 CDynamicObjectHeap<24,853>::CDynamicObjectHeap(bool): store the grow flag, clear the free list and block link, push all 853 embedded entries onto the free list (last entry first out)
#include <cstdio>
struct SEntry { SEntry* NextEntry; unsigned char Pad[24 - 4]; };
struct SBlock { SEntry Blocks[853]; SBlock* NextEntry; };
struct SHeap {
    SEntry* FreeList; SBlock StartBlock; bool AllowHeapToGrow;
    SHeap(bool allowHeapToGrow)
    {
        AllowHeapToGrow = allowHeapToGrow;
        FreeList = 0;
        StartBlock.NextEntry = 0;
        for (long i = 0; i < 853; ++i) {
            SEntry* entry = &StartBlock.Blocks[i];
            entry->NextEntry = FreeList;
            FreeList = entry;
        }
    }
};
int main()
{

    SHeap* h = new SHeap(true);
    if (!h->AllowHeapToGrow || h->StartBlock.NextEntry != 0 || h->FreeList != &h->StartBlock.Blocks[852]) { std::printf("00af8700_TEST FAIL\n"); return 1; }
    int n = 0;
    for (SEntry* e = h->FreeList; e; e = e->NextEntry) {
        if (e != &h->StartBlock.Blocks[852 - n]) { std::printf("00af8700_TEST FAIL\n"); return 1; }
        ++n;
    }
    if (n != 853) { std::printf("00af8700_TEST FAIL\n"); return 1; }
    delete h;
    SHeap* g = new SHeap(false);
    if (g->AllowHeapToGrow) { std::printf("00af8700_TEST FAIL\n"); return 1; }
    delete g;
    std::printf("00af8700_TEST PASS\n");
    return 0;
}