// Behaviour test for 00af88a0 CDynamicObjectHeap<24,853>::CreateMoreBlocks (copy of the source model).
#include <cstdio>

struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase() {}
    long                    AllocCount;
    long                    TotalAllocatorBlocks;
    bool                    RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
};

template<long ObjectSize, long BlockCount>
struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[ObjectSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[BlockCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    CObjectEntry*              FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool                       AllowHeapToGrow;
    void FreeObject(CObjectEntry* entry)
    {
        entry->NextEntry = FreeList;
        FreeList = entry;
    }
    void CreateMoreBlocks();
};

template<long ObjectSize, long BlockCount>
void CDynamicObjectHeap<ObjectSize, BlockCount>::CreateMoreBlocks()
{
    ++TotalAllocatorBlocks;
    CObjectAllocatorBlockEntry* block = new CObjectAllocatorBlockEntry;
    block->NextEntry = StartBlock.NextEntry;
    StartBlock.NextEntry = block;
    CObjectEntry* entry = block->Blocks;
    for (long i = 0; i < BlockCount; ++i, ++entry) {
        entry->NextEntry = FreeList;
        FreeObject(entry);
    }
}

typedef CDynamicObjectHeap<24, 853> THeap;

static long CountFree(THeap* h)
{
    long n = 0;
    for (THeap::CObjectEntry* e = h->FreeList; e; e = e->NextEntry) ++n;
    return n;
}

int main()
{
    THeap* h = new THeap;
    h->AllocCount = 0;
    h->TotalAllocatorBlocks = 0;
    h->FreeList = 0;
    h->StartBlock.NextEntry = 0;
    int ok = 1;
    h->CreateMoreBlocks();
    THeap::CObjectAllocatorBlockEntry* b1 = h->StartBlock.NextEntry;
    if (h->TotalAllocatorBlocks != 1 || b1 == 0 || b1->NextEntry != 0) ok = 0;
    if (ok && h->FreeList != &b1->Blocks[853 - 1]) ok = 0;
    if (ok && b1->Blocks[0].NextEntry != 0) ok = 0;
    if (ok && 853 > 1 && b1->Blocks[1].NextEntry != &b1->Blocks[0]) ok = 0;
    if (ok && CountFree(h) != 853) ok = 0;
    h->CreateMoreBlocks();
    THeap::CObjectAllocatorBlockEntry* b2 = h->StartBlock.NextEntry;
    if (ok && (h->TotalAllocatorBlocks != 2 || b2 == b1 || b2->NextEntry != b1)) ok = 0;
    if (ok && b2->Blocks[0].NextEntry != &b1->Blocks[853 - 1]) ok = 0;
    if (ok && CountFree(h) != 2 * 853) ok = 0;
    if (ok && h->AllocCount != 0) ok = 0;
    delete b2;
    delete b1;
    delete h;
    if (!ok) { std::printf("00af88a0_TEST FAIL\n"); return 1; }
    std::printf("00af88a0_TEST PASS\n");
    return 0;
}