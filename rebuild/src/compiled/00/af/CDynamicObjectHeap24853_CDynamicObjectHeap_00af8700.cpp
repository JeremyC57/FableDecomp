// CDynamicObjectHeap<24,853>::CDynamicObjectHeap(bool allowHeapToGrow) -- the fixed-size object heap's constructor: base heap constructor, the grow flag stored, the free list and the start block's link cleared, then the 853 embedded 24-byte entries pushed onto the free list
// Leaf: ??0?$CDynamicObjectHeap@$0BI@$0DFF@@@QAE@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8700, 68 bytes, __fastcall `ret 4`, returns this.
// Identity: FableWin.pdb name set `??0?$CDynamicObjectHeap@$0BI@$0DFF@@@QAE@_N@Z`; the vtable it stores, 0x0129F638, has the
//   RTTI type descriptor `.?AV?$CDynamicObjectHeap@$0BI@$0DFF@@@` (CDynamicObjectHeap<24,853>; slot 0 the deleting
//   destructor 0x00AF8A40, slot 1 0x00AF8780). No direct call sites (the global heap instances are built by the static
//   initialisers at 0x012244F5.., which inline the same body).
// Callees: CDynamicObjectHeapBase::CDynamicObjectHeapBase() 0x00F36C60 (landed).
// Layout: Ego_r.pdb CDynamicObjectHeap<N,M>: base CDynamicObjectHeapBase (0x14: vptr, AllocCount, TotalAllocatorBlocks,
//   RefCountReleaseEnabled, HeapNext), FreeList +0x14, StartBlock +0x18 = {CObjectEntry Blocks[853] (0x18 each),
//   NextEntry +0x4ff8 -> +0x5010}, AllowHeapToGrow +0x5014. The same model as the landed CreateMoreBlocks 0x00AF88A0 and
//   the <116,529> constructor 0x00AF8B40.
// Label: catalog "CDynamicObjectHeapBase_Construct_Small" is descriptive.
// Shape: as 0x00AF8B40: the flag, FreeList and StartBlock.NextEntry assigned in the body (scheduled before the vtable store),
//   then the push-front loop as a counted `dec ecx ; jne` with FreeList re-read every iteration.
class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;                     // +0x04
    long                    TotalAllocatorBlocks;           // +0x08
    bool                    RefCountReleaseEnabled;         // +0x0c
    CDynamicObjectHeapBase* HeapNext;                       // +0x10
    CDynamicObjectHeapBase();                               // 0x00F36C60
};

template <long ObjectSize, long BlockCount>
class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[ObjectSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[BlockCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    CObjectEntry*              FreeList;                    // +0x14
    CObjectAllocatorBlockEntry StartBlock;                  // +0x18
    bool                       AllowHeapToGrow;
    CDynamicObjectHeap(bool allowHeapToGrow);
    virtual ~CDynamicObjectHeap();
};

template <long ObjectSize, long BlockCount>
CDynamicObjectHeap<ObjectSize, BlockCount>::CDynamicObjectHeap(bool allowHeapToGrow)
{
    AllowHeapToGrow = allowHeapToGrow;
    FreeList = 0;
    StartBlock.NextEntry = 0;
    for (long i = 0; i < BlockCount; ++i) {
        CObjectEntry* entry = &StartBlock.Blocks[i];
        entry->NextEntry = FreeList;
        FreeList = entry;
    }
}

template class CDynamicObjectHeap<24, 853>;