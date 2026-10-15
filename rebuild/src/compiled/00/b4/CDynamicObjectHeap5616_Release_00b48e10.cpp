// CDynamicObjectHeap<56,16>::Release() -- free every allocator block chained after the heap's embedded start block and empty the free list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B48E10, length 59 bytes.
// Callees: operator delete @ 0x00BFE9BC (`add esp,4`).
// The manifest label "CQuadTreeElement::RemoveObjectCollections" is propagated and rejected: the body walks the
// chain at +0x398 with each block's next pointer at +0x380 and zeroes +0x14 -- PDB CDynamicObjectHeap<56,16>
// (0x3a0 bytes): FreeList +0x14, StartBlock (CObjectAllocatorBlockEntry: CObjectEntry Blocks[16] of 56 bytes, then
// NextEntry at +0x380) at +0x18, so StartBlock.NextEntry is +0x398; AllowHeapToGrow +0x39c. FableWin.pdb names
// the private virtual ?Release@?$CDynamicObjectHeap@$0DI@$0BA@@@EAEXXZ; the sibling 0x00B48C70 is the <12,16> twin
// (NextEntry +0xd8 / +0xc0) and the landed CDynamicObjectHeap<56,16>::Allocate (0x00B49660) uses the same offsets.
// Shape: the chain head is tested once on entry, the rotated loop (aligned with `mov edi,edi`) re-reads it, keeps
// the next block in edi across the delete, stores it back and tests it through eax; FreeList is cleared last.
struct CObjectEntry {
    CObjectEntry* NextEntry;
    unsigned char Pad[52];
};

struct CObjectAllocatorBlockEntry {
    CObjectEntry                Blocks[16];        // +0x000
    CObjectAllocatorBlockEntry* NextEntry;         // +0x380
};

struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;// +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
};

template<long ObjectSize, long BlockCount>
struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    CObjectEntry*              FreeList;           // +0x14
    CObjectAllocatorBlockEntry StartBlock;         // +0x18
    bool                       AllowHeapToGrow;    // +0x39c
    virtual void Release();
};

template<long ObjectSize, long BlockCount>
void CDynamicObjectHeap<ObjectSize, BlockCount>::Release()
{
    while (StartBlock.NextEntry) {
        CObjectAllocatorBlockEntry* next = StartBlock.NextEntry->NextEntry;
        delete StartBlock.NextEntry;
        StartBlock.NextEntry = next;
    }
    FreeList = 0;
}

template void CDynamicObjectHeap<56, 16>::Release();