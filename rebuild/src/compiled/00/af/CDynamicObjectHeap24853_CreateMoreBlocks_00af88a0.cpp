// CDynamicObjectHeap<24,853>::CreateMoreBlocks() -- allocate one more allocator block, chain it after the start block and push its 853 entries onto the free list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF88A0, length 69 bytes (80 with int3 padding).
// Leaf: CreateMoreBlocks
// Callees: operator new @ 0x00BFEA1A (`push 0x4ffc`, `add esp,4`).
// The catalog label "CDynamicObjectHeap<24" is right about the object size: the block is 853 entries of 24
// bytes (0x355 * 0x18 = 0x4ff8) plus the block link at +0x4ff8, so the heap's StartBlock.NextEntry sits at
// 0x18 + 0x4ff8 = 0x5010. Layout from the PDB CDynamicObjectHeap<N,M> template (base CDynamicObjectHeapBase
// 20 bytes: AllocCount +4, TotalAllocatorBlocks +8; FreeList +0x14; StartBlock +0x18), the same model as the
// landed CDynamicObjectHeap<56,16>::Release (0x00B48E10) and the <116,529> constructor (0x00AF8B40).
// Shape: ++TotalAllocatorBlocks is a load/inc/store split around the size push; the new block is linked in
// front of StartBlock.NextEntry; then a counted loop (`dec` ; `jne`; VC unrolls it 4x when the count divides) walking an entry pointer -- an indexed `&Blocks[i]` form schedules the first two unrolled lea's differently -- in which each entry's
// link is written TWICE from a re-read FreeList (the entry's own link initialised to the free-list head, then
// the inline free-list push `entry->NextEntry = FreeList; FreeList = entry;`) -- the store through the entry
// may alias FreeList, so VC re-reads it.
struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;// +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
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
    CObjectEntry*              FreeList;           // +0x14
    CObjectAllocatorBlockEntry StartBlock;         // +0x18
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

template void CDynamicObjectHeap<24, 853>::CreateMoreBlocks();