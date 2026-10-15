// CDynamicObjectHeapBase::RefCountedReleaseAllHeaps() -- walk the global heap list and arm ref-counted release on every heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36CC0, length 43 bytes. No callers (an unreferenced shutdown helper kept
// by the linker); split from catalogue row 00f36ca0 via the skipped-row ledger, chunk 907. The list head is the global at
// 0x0143E964 (see the constructor @ 0x00F36C60, which links each new heap at the head).
// Callees: each heap's virtual slot 1 (+0x04) through the inlined RefCountedRelease body (0x00F36CA0).
// Shape: the cursor lives in esi; the loop is entered through `jmp` over the `lea ecx,[ecx]` alignment pad; the inline
// body reads AllocCount, stores the flag, and calls slot 1 only when the count is zero.
struct CDynamicObjectHeapBase {                      // PDB: 20 bytes, vtable 0x01301CD4
    virtual ~CDynamicObjectHeapBase();                   // slot 0 (the deleting destructor @ 0x00F36CF0)
    virtual void FreeAllBlocks();                        // slot 1 (+0x04): give the heap's blocks back once nothing is allocated
    long                    AllocCount;                  // +0x04 live allocations
    long                    TotalAllocatorBlocks;        // +0x08
    bool                    RefCountReleaseEnabled;      // +0x0c once set, the last Free() releases the heap
    CDynamicObjectHeapBase* HeapNext;                    // +0x10 global singly linked list of heaps
    void RefCountedRelease()                             // inlined here; the out-of-line copy is 0x00F36CA0
    {
        RefCountReleaseEnabled = true;
        if (AllocCount == 0)
            FreeAllBlocks();
    }
    static void RefCountedReleaseAllHeaps();
};

extern CDynamicObjectHeapBase* g_pHeapList_0143e964;     // 0x0143E964

void CDynamicObjectHeapBase::RefCountedReleaseAllHeaps()
{
    for (CDynamicObjectHeapBase* heap = g_pHeapList_0143e964; heap != 0; heap = heap->HeapNext)
        heap->RefCountedRelease();
}