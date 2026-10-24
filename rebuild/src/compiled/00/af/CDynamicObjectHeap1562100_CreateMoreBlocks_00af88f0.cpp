// CDynamicObjectHeap<156,2100>::CreateMoreBlocks() -- grow the object heap by one allocator block: count it, allocate the 2100-entry block, link it after the embedded start block and push every new 156-byte entry onto the free list
// Leaf: ?CreateMoreBlocks@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF88F0, length 176 bytes, plain `ret`. The catalog's CDynamicObjectHeap<164,...>
// label is the Ego_r.pdb instantiation (CDynamicObjectHeap<164,1998>); the retail block is 0x4FFB4 bytes = 2100 entries of 0x9c
// plus the link word, so the TLC instantiation is <156,2100> (the six-way unrolled loop runs 0x15E = 350 times).
// Callees: operator new @ 0x00BFEA1A.
// Layout: as the landed CDynamicObjectHeap_116_529 ctor 0x00AF8B40 (CDynamicObjectHeapBase 0x14: TotalAllocatorBlocks +8; FreeList
// +0x14; StartBlock +0x18 = { CObjectEntry Blocks[2100]; NextEntry } so StartBlock.NextEntry is +0x4FFC8).
// Shape: the new block is linked in front of the start block's chain; each entry gets its link set and is then released
// through the inline Free (link = FreeList; FreeList = entry), so the link is written twice -- the second FreeList read is a
// reload because the first store through the entry may alias the member.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A

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