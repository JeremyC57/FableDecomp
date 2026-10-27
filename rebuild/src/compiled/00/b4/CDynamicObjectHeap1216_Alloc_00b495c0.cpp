// CDynamicObjectHeap<12,16>::Alloc(long) -- pop the free-list head, first growing the heap when the list is empty and growth is allowed
// Leaf: ?Alloc@?$CDynamicObjectHeap@$0M@$0BA@@@QAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B495C0, 46 bytes, thiscall, `ret 4` (the size argument is unused).
// Identity (proven): the AllowHeapToGrow flag it tests sits at +0xDC = 0x18 + 16 x 12 + 4 and its callee is the landed
//   CDynamicObjectHeap<12,16>::CreateMoreBlocks 0x00B48F20; Ego_r/FableWin.pdb ?Alloc@?$CDynamicObjectHeap@...@@QAEPAXJ@Z.
//   No direct callers (each heap's operator new inlines it).
// Callees: CDynamicObjectHeap<12,16>::CreateMoreBlocks 0x00B48F20.
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
    bool AllowHeapToGrow;                                // +0xDC
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
template void* CDynamicObjectHeap<12, 16>::Alloc(long);