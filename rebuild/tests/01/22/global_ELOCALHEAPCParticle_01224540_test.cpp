#include <cstdio>
#include <cstring>
#include <cmath>
template <unsigned int TSize, unsigned int TCount> class CDynamicObjectHeap {
public:
    struct CObjectEntry { CObjectEntry* NextEntry; unsigned char Pad[TSize - sizeof(void*)]; };
    struct CBlock { CObjectEntry Blocks[TCount]; CBlock* NextEntry; };
    CObjectEntry* FreeList; CBlock StartBlock; bool AllowHeapToGrow;
    CDynamicObjectHeap(bool grow)
    {
        AllowHeapToGrow = grow; FreeList = 0; StartBlock.NextEntry = 0;
        for (int i = 0; i < (int)TCount; ++i) { StartBlock.Blocks[i].NextEntry = FreeList; FreeList = &StartBlock.Blocks[i]; }
    }
};

int main()
{
    CDynamicObjectHeap<156, 2100>* h = new CDynamicObjectHeap<156, 2100>(false);
    if (h->AllowHeapToGrow || h->StartBlock.NextEntry != 0 || h->FreeList != &h->StartBlock.Blocks[2099]) do { std::printf("01224540_TEST FAIL\n"); return 1; } while (0);
    int n = 0;                                            // the free list runs from the last entry back to the first
    for (CDynamicObjectHeap<156, 2100>::CObjectEntry* e = h->FreeList; e; e = e->NextEntry) {
        if (e != &h->StartBlock.Blocks[2099 - n]) do { std::printf("01224540_TEST FAIL\n"); return 1; } while (0);
        ++n;
    }
    if (n != 2100) do { std::printf("01224540_TEST FAIL\n"); return 1; } while (0);
    delete h;
    std::printf("01224540_TEST PASS\n");
    return 0;
}