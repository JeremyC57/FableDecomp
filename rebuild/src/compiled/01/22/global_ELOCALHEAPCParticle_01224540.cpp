// _global::$E_LOCAL_HEAP_CParticle() -- dynamic initializer of the particle object heap: the inline CDynamicObjectHeap<156,2100>(false) constructor (base, vtable, no growth, every embedded entry pushed onto the free list), then its destructor is registered with atexit
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01224540, 73 bytes, in `.text$yc`, reached from the .CRT$XCU table, plain `ret`.
// Identity (proven): Ego_r.pdb / FableWin.pdb `dynamic initializer for 'LOCAL_HEAP_CParticle'`; the object 0x013D7E50 gets the
//   vtable 0x0129F644 = CDynamicObjectHeap<156,2100> (rtti_bases.tsv `.?AV?$CDynamicObjectHeap@$0JM@$0IDE@@@`) and is the heap
//   NParticleEngine::CParticle::operator delete 0x00AF8650 returns particles to; the registered thunk 0x0122AC90 is the inline
//   ~CDynamicObjectHeap (vtable, Release 0x00AF8860, ~CDynamicObjectHeapBase 0x00F36C90). The neighbouring heap 0x01427E20
//   (vtable 0x0129F650, thunk 0x0122ACB0) is the next LOCAL_HEAP_* object.
// Label: the manifest's "sub_01224540" is a gap-scan placeholder; the landed thunk 0x0122AC90 carries a wrong
//   "CTCInventoryItem::ClearCachedDefs" label.
// Callees: CDynamicObjectHeapBase::CDynamicObjectHeapBase() 0x00F36C60, _atexit 0x004012BC.
// Layout: as the landed CDynamicObjectHeap<156,2100> members (0x00AF87C0 / 0x00AF8860): base 0x14 bytes, FreeList +0x14,
//   StartBlock +0x18 { Blocks[2100] of 0x9c; NextEntry +0x4FFC8 }, AllowHeapToGrow +0x4FFCC.
// Shape: the landed out-of-line constructor body 0x00AF87C0 inline; against constant addresses VC7.1 walks the entries with
//   one pointer compared with the end address (no six-way unroll) and stores FreeList once after the loop. The constructor
//   must be `__forceinline`: VC7.1 calls a plain inline one holding a loop out of line.
class CDynamicObjectHeapBase {
public:
    CDynamicObjectHeapBase();                            // 0x00F36C60
    virtual ~CDynamicObjectHeapBase();                   // 0x00F36C90
    long AllocCount;                                     // +0x04
    long TotalAllocatorBlocks;                           // +0x08
    bool RefCountReleaseEnabled;                         // +0x0C
    CDynamicObjectHeapBase* HeapNext;                    // +0x10
};

template <unsigned long TSize, unsigned long TCount> class CDynamicObjectHeap : public CDynamicObjectHeapBase {
public:
    struct CObjectEntry {
        CObjectEntry* NextEntry;
        unsigned char Pad[TSize - 4];
    };
    struct CObjectAllocatorBlockEntry {
        CObjectEntry                Blocks[TCount];
        CObjectAllocatorBlockEntry* NextEntry;
    };
    __forceinline CDynamicObjectHeap(bool allowHeapToGrow)
    {
        AllowHeapToGrow = allowHeapToGrow;
        FreeList = 0;
        StartBlock.NextEntry = 0;
        for (long i = 0; i < (long)TCount; ++i) {
            StartBlock.Blocks[i].NextEntry = FreeList;
            FreeList = &StartBlock.Blocks[i];
        }
    }
    virtual ~CDynamicObjectHeap() { Release(); }
private:
    virtual void Release();                              // 0x00AF8860 for <156,2100>
public:
    CObjectEntry*              FreeList;                 // +0x14
    CObjectAllocatorBlockEntry StartBlock;               // +0x18
    bool                       AllowHeapToGrow;          // +0x4FFCC for <156,2100>
};

CDynamicObjectHeap<156, 2100> LOCAL_HEAP_CParticle(false);   // 0x013D7E50