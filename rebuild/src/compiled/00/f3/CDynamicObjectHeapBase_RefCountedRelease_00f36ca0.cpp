// CDynamicObjectHeapBase::RefCountedRelease() -- arm ref-counted release of this heap and free its blocks now if nothing is allocated
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36CA0, length 17 bytes (the catalogue row 00f36ca0 also held the four
// functions at 0x00F36CC0, 0x00F36CF0, 0x00F36D10 and 0x00F36D20, split via the skipped-row ledger, chunk 907).
// Callers: twenty derived-heap Free() bodies in 0x00AF8xxx / 0x00B44xxx-0x00B4Axxx, each after its own
// `AllocCount == 0 && RefCountReleaseEnabled` test (0x00AF8763). Callees: the heap's own virtual slot 1 (+0x04).
// Shape: AllocCount is read into eax before the flag store, the store happens, and the branch decides between a
// plain `ret` and the tail jump through the vtable; the slot-1 call is the only call, so it is a `jmp`.
struct CDynamicObjectHeapBase {                      // PDB: 20 bytes, vtable 0x01301CD4
    virtual ~CDynamicObjectHeapBase();                   // slot 0 (the deleting destructor @ 0x00F36CF0)
    virtual void FreeAllBlocks();                        // slot 1 (+0x04): give the heap's blocks back once nothing is allocated
    long                    AllocCount;                  // +0x04 live allocations
    long                    TotalAllocatorBlocks;        // +0x08
    bool                    RefCountReleaseEnabled;      // +0x0c once set, the last Free() releases the heap
    CDynamicObjectHeapBase* HeapNext;                    // +0x10 global singly linked list of heaps
    void RefCountedRelease();
};

void CDynamicObjectHeapBase::RefCountedRelease()
{
    RefCountReleaseEnabled = true;
    if (AllocCount == 0)
        FreeAllBlocks();
}