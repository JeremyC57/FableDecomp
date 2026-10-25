// CDynamicObjectHeap<116,529>::CDynamicObjectHeap(bool allowHeapToGrow) -- the fixed-size object heap's constructor: base heap constructor (0x00F36C60), the grow flag stored, the free list and the start block's link cleared, then the 529 embedded 116-byte entries pushed onto the free list
// Leaf: ??0?$CDynamicObjectHeap@$0HE@$0CBB@@@QAE@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF8B40, 68 bytes, __fastcall `ret 4`, returns this.
// Identity: the vtable it stores, 0x0129F650, has the RTTI type descriptor `.?AV?$CDynamicObjectHeap@$0HE@$0CBB@@@`
//   (CDynamicObjectHeap<116,529>); the same template as the landed <24,853> constructor 0x00AF8700 (FableWin.pdb
//   `??0?$CDynamicObjectHeap@$0BI@$0DFF@@@QAE@_N@Z`) and the landed <116,529> CreateMoreBlocks 0x00AF8C00.
//   Renamed from the non-template stand-in CDynamicObjectHeap_116_529 (the chunk label
//   CSpecialAbilitiesGhostSwordDef::operator= was propagated garbage).
// Callees: CDynamicObjectHeapBase::CDynamicObjectHeapBase() 0x00F36C60 (landed).
// Layout: Ego_r.pdb CDynamicObjectHeap<N,M>: base CDynamicObjectHeapBase (0x14), FreeList +0x14, StartBlock +0x18 =
//   {CObjectEntry Blocks[529] (0x74 each), NextEntry +0xefcc}, AllowHeapToGrow +0xefd0.
// Shape: the flag, FreeList and StartBlock.NextEntry assigned in the body (scheduled before the vtable store), then the
//   push-front loop as a counted `dec ecx ; jne` with FreeList re-read every iteration.
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

template class CDynamicObjectHeap<116, 529>;