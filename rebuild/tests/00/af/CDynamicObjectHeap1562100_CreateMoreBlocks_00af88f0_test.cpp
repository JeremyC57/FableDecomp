#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#endif

struct CDynamicObjectHeapBase {
    virtual void v00();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
};

template<unsigned long TSize, unsigned long TCount> struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[TSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[TCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    CObjectEntry*              FreeList;           // +0x14
    CObjectAllocatorBlockEntry StartBlock;         // +0x18
    bool                       AllowHeapToGrow;
    void Free(CObjectEntry* entry)
    {
        entry->NextEntry = FreeList;
        FreeList = entry;
    }
    void CreateMoreBlocks();
};

template<unsigned long TSize, unsigned long TCount> void CDynamicObjectHeap<TSize, TCount>::CreateMoreBlocks()
{
    ++TotalAllocatorBlocks;
    CObjectAllocatorBlockEntry* block = new CObjectAllocatorBlockEntry;
    block->NextEntry = StartBlock.NextEntry;
    StartBlock.NextEntry = block;
    for (unsigned long i = 0; i < TCount; ++i) {
        CObjectEntry* entry = &block->Blocks[i];
        entry->NextEntry = FreeList;
        Free(entry);
    }
}

template void CDynamicObjectHeap<156, 2100>::CreateMoreBlocks();

void CDynamicObjectHeapBase::v00() { }
typedef CDynamicObjectHeap<156, 2100> CHeap;
int main()
{
    CHeap* heap = (CHeap*)std::calloc(1, sizeof(CHeap));
    CHeap::CObjectEntry sentinel;
    heap->FreeList = &sentinel;
    heap->TotalAllocatorBlocks = 1;
    heap->CreateMoreBlocks();
    CHeap::CObjectAllocatorBlockEntry* block = heap->StartBlock.NextEntry;
    if (heap->TotalAllocatorBlocks != 2 || block == 0 || block->NextEntry != 0 || heap->FreeList != &block->Blocks[2099]) { std::printf("00af88f0_TEST FAIL\n"); return 1; }
    long n = 0;
    for (CHeap::CObjectEntry* e = heap->FreeList; e != &sentinel; e = e->NextEntry) {
        if (e != &block->Blocks[2099 - n]) { std::printf("00af88f0_TEST FAIL\n"); return 1; }
        ++n;
    }
    if (n != 2100) { std::printf("00af88f0_TEST FAIL\n"); return 1; }
    heap->CreateMoreBlocks();                        // the second new block goes in front of the first
    if (heap->StartBlock.NextEntry == block || heap->StartBlock.NextEntry->NextEntry != block || heap->TotalAllocatorBlocks != 3) { std::printf("00af88f0_TEST FAIL\n"); return 1; }
    std::printf("00af88f0_TEST PASS\n");
    return 0;
}