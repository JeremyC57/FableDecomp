// CDynamicObjectHeap<156,2100>::Alloc(long) -- pop the free-list head, first growing the heap when the list is empty and growth is allowed
// Leaf: ?Alloc@?$CDynamicObjectHeap@$0JM@$0IDE@@@QAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8A10, 46 bytes, thiscall, `ret 4` (the size argument is unused).
// Identity (proven): the AllowHeapToGrow flag it tests sits at +0x4FFCC = 0x18 + 2100 x 156 + 4 and its callee is the landed
//   CDynamicObjectHeap<156,2100>::CreateMoreBlocks 0x00AF88F0; Ego_r/FableWin.pdb ?Alloc@?$CDynamicObjectHeap@...@@QAEPAXJ@Z.
//   No direct callers (each heap's operator new inlines it).
// Callees: CDynamicObjectHeap<156,2100>::CreateMoreBlocks 0x00AF88F0.
// Shape (family K055, representative 0x00AF89C0): the landed CDynamicObjectHeap<56,16> allocator 0x00B49660: `if (grow)
//   CreateMoreBlocks(); else return 0;`, then ++AllocCount and the head unlinked.
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
        CObjectAllocatorBlockEntry* NextEntry;
    };
    void* Alloc(long size);
    void CreateMoreBlocks();                             // bound public in reloc_bindings (Ego_r.pdb: private)
    CObjectEntry* FreeList;                              // +0x14
    CObjectAllocatorBlockEntry StartBlock;               // +0x18
    bool AllowHeapToGrow;                                // +0x4FFCC
};
template <long ObjectSize, long BlockCount>
void* CDynamicObjectHeap<ObjectSize, BlockCount>::Alloc(long size)
{
    if (FreeList == 0) {
        if (AllowHeapToGrow)
            CreateMoreBlocks();
        else
            return 0;
    }
    ++AllocCount;
    CObjectEntry* entry = FreeList;
    FreeList = entry->NextEntry;
    return entry;
}
template void* CDynamicObjectHeap<156, 2100>::Alloc(long);