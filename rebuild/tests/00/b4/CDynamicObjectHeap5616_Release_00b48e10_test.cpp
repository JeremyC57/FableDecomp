// Behaviour test for CDynamicObjectHeap<56,16>::Release() @ 0x00B48E10
#include <cstdio>

static int g_blocksDeleted = 0;

struct CObjectEntry {
    CObjectEntry* NextEntry;
    unsigned char Pad[52];
};

struct CObjectAllocatorBlockEntry {
    CObjectEntry                Blocks[16];
    CObjectAllocatorBlockEntry* NextEntry;
    ~CObjectAllocatorBlockEntry() { ++g_blocksDeleted; }
};

struct CDynamicObjectHeap_56_16 {
    long                       AllocCount;
    CObjectEntry*              FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool                       AllowHeapToGrow;
    void Release()
    {
        while (StartBlock.NextEntry) {
            CObjectAllocatorBlockEntry* next = StartBlock.NextEntry->NextEntry;
            delete StartBlock.NextEntry;
            StartBlock.NextEntry = next;
        }
        FreeList = 0;
    }
};

int main()
{
    CDynamicObjectHeap_56_16* heap = new CDynamicObjectHeap_56_16;
    CObjectAllocatorBlockEntry* a = new CObjectAllocatorBlockEntry;
    CObjectAllocatorBlockEntry* b = new CObjectAllocatorBlockEntry;
    a->NextEntry = b; b->NextEntry = 0;
    heap->StartBlock.NextEntry = a;
    heap->FreeList = &heap->StartBlock.Blocks[0];
    heap->Release();
    if (g_blocksDeleted != 2 || heap->StartBlock.NextEntry != 0 || heap->FreeList != 0) { std::printf("00b48e10_TEST FAIL\n"); return 1; }
    heap->FreeList = &heap->StartBlock.Blocks[1];
    heap->Release();                                   // no extra blocks: only the free list is cleared
    if (g_blocksDeleted != 2 || heap->FreeList != 0) { std::printf("00b48e10_TEST FAIL\n"); return 1; }
    std::printf("00b48e10_TEST PASS\n");
    return 0;
}