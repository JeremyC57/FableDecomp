// CDynamicObjectHeapBase::CDynamicObjectHeapBase() -- link this heap at the head of the global heap list and zero its counters and the ref-count-release flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36C60, length 34 bytes (+ int3 padding).
// Callees: none. Vtable 0x01301CD4 = CDynamicObjectHeapBase (rtti-vtables.tsv); the list head is the global at 0x0143E964. PDB CDynamicObjectHeapBase
// (20 bytes): AllocCount +4, TotalAllocatorBlocks +8, RefCountReleaseEnabled +0xc, HeapNext +0x10. Shape: the stores follow the body order
// (HeapNext from the global, the global from `this`, then the flag and the two counters sharing the ecx zero register) after the vtable
// store; `this` is copied to eax at the top because it is also the return value.
struct CDynamicObjectHeapBase {
    virtual void v00();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
    CDynamicObjectHeapBase();
};

extern CDynamicObjectHeapBase* g_pHeapList_0143e964;   // 0x0143E964

CDynamicObjectHeapBase::CDynamicObjectHeapBase()
{
    HeapNext = g_pHeapList_0143e964;
    g_pHeapList_0143e964 = this;
    RefCountReleaseEnabled = false;
    AllocCount = 0;
    TotalAllocatorBlocks = 0;
}