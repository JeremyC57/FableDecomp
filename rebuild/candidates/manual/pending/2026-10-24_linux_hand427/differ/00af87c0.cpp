// CDynamicObjectHeap<156,2100>::CDynamicObjectHeap(bool allowHeapToGrow) -- object-heap constructor: base heap constructor, grow flag, free list and block link cleared, then the 2100 embedded 156-byte entries pushed onto the free list
// Grade: DIFFER(115v111) -- in the six-way unrolled push-front loop VC7.1 folds the first entry's link store into `mov [eax-9Ch],edx` (6 bytes) and stores entry 2 before forming entry 3's address; retail forms `lea ecx,[eax-9Ch]` first and stores through ecx. Tried: index loop (long / unsigned long), inline Free(entry) helper, pointer walk (changes the unroll).
// Leaf: ??0?$CDynamicObjectHeap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF87C0, length 111 bytes, thiscall `ret 4`, `this` returned in eax.
// Identity: the vtable stored, 0x0129F644, is a CDynamicObjectHeap instantiation (rtti-vtables.tsv; slots 0x00AF8A70 /
//   0x00AF8860); the element stride 0x9C and the 0x15E x 6 unrolled loop are the <156,2100> heap whose CreateMoreBlocks
//   0x00AF88F0 is landed (StartBlock.NextEntry +0x4FFC8, AllowHeapToGrow +0x4FFCC). No direct caller (the static heap's
//   initialiser reaches it through a dynamic-initialiser thunk or it is unreferenced).
// Callees: CDynamicObjectHeapBase::CDynamicObjectHeapBase() @ 0x00F36C60 (landed).
// Layout: as the landed CDynamicObjectHeap_116_529 ctor 0x00AF8B40 (base 0x14 bytes; FreeList +0x14; StartBlock +0x18 =
//   { CObjectEntry Blocks[2100]; NextEntry }; the grow flag after it).
// Shape: the landed 116/529 constructor verbatim: the flag assigned first in the body, FreeList and NextEntry cleared,
//   all scheduled before the vtable store; VC7.1 unrolls the 2100-trip push-front loop six ways (2100 = 6 x 350), keeping
//   FreeList in a register inside each group of six.
// Label: catalog "CDynamicObjectHeapBase_Construct_Large" is behavioural.
struct CDynamicObjectHeapBase {
    virtual void v00();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
    CDynamicObjectHeapBase();                      // 0x00F36C60
};

template<unsigned long TSize, unsigned long TCount> struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[TSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[TCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    CObjectEntry*              FreeList;           // +0x14
    CObjectAllocatorBlockEntry StartBlock;         // +0x18
    bool                       AllowHeapToGrow;    // +0x18 + TCount * TSize + 4
    virtual void v00();
    CDynamicObjectHeap(bool allowHeapToGrow);
};

template<unsigned long TSize, unsigned long TCount>
CDynamicObjectHeap<TSize, TCount>::CDynamicObjectHeap(bool allowHeapToGrow)
{
    AllowHeapToGrow = allowHeapToGrow;
    FreeList = 0;
    StartBlock.NextEntry = 0;
    for (long i = 0; i < (long)TCount; ++i) {
        CObjectEntry* entry = &StartBlock.Blocks[i];
        entry->NextEntry = FreeList;
        FreeList = entry;
    }
}

template CDynamicObjectHeap<156, 2100>::CDynamicObjectHeap(bool allowHeapToGrow);
