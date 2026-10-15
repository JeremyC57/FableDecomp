// CDynamicObjectHeapBase::~CDynamicObjectHeapBase() [scalar deleting destructor] -- store the vtable, free the object when bit 0 of the flags says so
// Leaf: ??_GCDynamicObjectHeapBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36CF0, length 31 bytes (vtable 0x01301CD4 slot 0); split from catalogue row
// 00f36ca0 via the skipped-row ledger, chunk 907. Callees: operator delete @ 0x00BFE9BC.
// Shape: the compiler-generated `??_G` of an empty virtual destructor: `test [esp+4],1`, the vtable store, the
// conditional `push this ; call operator delete ; add esp,4`, and `this` returned in eax with `ret 4`.
struct CDynamicObjectHeapBase {                      // PDB: 20 bytes, vtable 0x01301CD4
    virtual ~CDynamicObjectHeapBase();                   // slot 0 (the deleting destructor @ 0x00F36CF0)
    virtual void FreeAllBlocks();                        // slot 1 (+0x04): give the heap's blocks back once nothing is allocated
    long                    AllocCount;                  // +0x04 live allocations
    long                    TotalAllocatorBlocks;        // +0x08
    bool                    RefCountReleaseEnabled;      // +0x0c once set, the last Free() releases the heap
    CDynamicObjectHeapBase* HeapNext;                    // +0x10 global singly linked list of heaps
};

CDynamicObjectHeapBase::~CDynamicObjectHeapBase()
{
}
