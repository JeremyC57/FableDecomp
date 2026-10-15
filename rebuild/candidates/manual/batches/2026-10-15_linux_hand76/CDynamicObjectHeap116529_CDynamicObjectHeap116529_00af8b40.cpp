// CDynamicObjectHeap_116_529::CDynamicObjectHeap_116_529(bool allowHeapToGrow) -- CDynamicObjectHeap<116,529> constructor: base heap constructor (0x00F36C60), free list and block link cleared, the grow flag stored, then the 529 embedded 116-byte entries pushed onto the free list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF8B40, length 68 bytes.
// Leaf: ??0CDynamicObjectHeap_116_529
// The chunk label (CSpecialAbilitiesGhostSwordDef::operator=) is propagated garbage: the vtable stored here,
// 0x0129F650, is a CDynamicObjectHeap instantiation (rtti-vtables.tsv), and the body is its constructor.
// Callees: 0x00F36C60 (CDynamicObjectHeapBase::CDynamicObjectHeapBase, landed). Layout from the PDB
// CDynamicObjectHeap<N, M> template (base 20 bytes; FreeList +0x14, StartBlock +0x18 = { CObjectEntry
// Blocks[M]; NextEntry }, AllowHeapToGrow after it): 0x18 + 529 * 0x74 = 0xEFCC is StartBlock.NextEntry and
// 0xEFD0 the flag.
// Shape: the three scalar stores (flag from the argument through al, FreeList, NextEntry) all scheduled before
// the vtable store in BODY order -- the flag is assigned first in the body (as initialiser-list members the
// flag is loaded into cl and stored after FreeList); then the push-front loop as a counted `dec ecx ; jne` with the entry pointer in eax stepping by
// 0x74 and FreeList re-read from memory every iteration (the store through the entry may alias it); the
// aligned loop head is entered with a short jmp.
struct CDynamicObjectHeapBase {
    virtual void v00();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
    CDynamicObjectHeapBase();                      // 0x00F36C60
};

struct CObjectEntry_116 {                          // a free 116-byte slot: the link word first
    CObjectEntry_116* NextEntry;
    unsigned char     Pad[0x70];
};

struct CObjectAllocatorBlockEntry_116_529 {
    CObjectEntry_116                     Blocks[529];   // +0x00
    CObjectAllocatorBlockEntry_116_529*  NextEntry;     // +0xefb4
};

struct CDynamicObjectHeap_116_529 : CDynamicObjectHeapBase {   // vtable 0x0129F650
    CObjectEntry_116*                   FreeList;          // +0x14
    CObjectAllocatorBlockEntry_116_529  StartBlock;        // +0x18
    bool                                AllowHeapToGrow;   // +0xefd0
    virtual void v00();
    CDynamicObjectHeap_116_529(bool allowHeapToGrow);
};

CDynamicObjectHeap_116_529::CDynamicObjectHeap_116_529(bool allowHeapToGrow)
{
    AllowHeapToGrow = allowHeapToGrow;
    FreeList = 0;
    StartBlock.NextEntry = 0;
    for (long i = 0; i < 529; ++i) {
        CObjectEntry_116* entry = &StartBlock.Blocks[i];
        entry->NextEntry = FreeList;
        FreeList = entry;
    }
}
