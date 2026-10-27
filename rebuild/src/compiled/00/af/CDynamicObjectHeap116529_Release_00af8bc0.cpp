// CDynamicObjectHeap<116,529>::Release() -- free every allocator block chained after the heap's embedded start block, then empty the free list
// Leaf: ?Release@?$CDynamicObjectHeap@$0HE@$0CBB@@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8BC0, 59 bytes, thiscall, plain `ret`.
// Identity (proven): slot 1 of the CDynamicObjectHeap vtable 0x0129F650; StartBlock.NextEntry at +0xEFCC = 0x18 + 529 x 116
//   (each block's link at +0xEFB4); the landed <116,529> ctor 0x00AF8B40 / CreateMoreBlocks 0x00AF8C00 (StartBlock.NextEntry +0xEFCC).
// Callees: operator delete 0x00BFE9BC.
// Shape (family K054, representative 0x00AF8780): the landed CDynamicObjectHeap<56,16>::Release 0x00B48E10 verbatim.
class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();
    long AllocCount;                                     // +0x04
    long TotalAllocatorBlocks;                           // +0x08
    bool RefCountReleaseEnabled;                         // +0x0C
    CDynamicObjectHeapBase* HeapNext;                    // +0x10
};
template <long ObjectSize, long BlockCount> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[ObjectSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry Blocks[BlockCount];                 // +0x000
        CObjectAllocatorBlockEntry* NextEntry;           // +0xEFB4 here
    };
private:
    virtual void Release();
public:
    CObjectEntry* FreeList;                              // +0x14
    CObjectAllocatorBlockEntry StartBlock;               // +0x18
    bool AllowHeapToGrow;
};
template <long ObjectSize, long BlockCount>
void CDynamicObjectHeap<ObjectSize, BlockCount>::Release()
{
    while (StartBlock.NextEntry) {
        CObjectAllocatorBlockEntry* next = StartBlock.NextEntry->NextEntry;
        delete StartBlock.NextEntry;
        StartBlock.NextEntry = next;
    }
    FreeList = 0;
}
template class CDynamicObjectHeap<116, 529>;