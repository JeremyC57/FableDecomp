// CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity) -- CBaseClassNonCopyable base, the growth granularity rounded up to 16, this class's vtable, the free-area root cleared, no blocks, the default 0x400 capacity replaced by the rounded request when one is given
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- CArea without an inline constructor, the root's two words and FirstMemoryBlock/Capacity assigned in the body (GrowthGranularity in the initialiser list): the zero register is set before the GrowthGranularity store as in retail
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5C570, length 74 bytes.
// Leaf: ??0CSimpleMemoryPool
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0.
// Layout: engine/CSimpleMemoryPool.h (FirstMemoryBlock +4, Capacity +8, GrowthGranularity +0xc, Root (CArea) +0x10).
// The generated header is a flat record with no virtual functions or constructors, so a constructor that stores the
// vtable (0x0129A064 = CSimpleMemoryPool in rtti-vtables.tsv) cannot be expressed on it: local polymorphic model with
// the header's member names and offsets.
// Shape: the granularity is the only initialiser (stored before the vtable), Root is a member with an inline
// constructor (its two words after the vtable), FirstMemoryBlock and Capacity are body assignments sharing the ecx
// zero; the capacity request goes through the inline SetCapacity (0x00A5C5C0: `(n + 15) / 16 * 16`).
struct CBaseClassNonCopyable {                  // vtable 0x0129A7C4
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                    // 0x0099A2F0
};

struct CMemoryBlock;

struct CArea {
    unsigned long Size;                         // +0x00
    CArea*        NextFreeArea;                 // +0x04
    long          Status;                       // +0x08 (EAreaStatus, not initialised)
};

struct CSimpleMemoryPool : CBaseClassNonCopyable {      // vtable 0x0129A064
    CMemoryBlock* FirstMemoryBlock;             // +0x04
    unsigned long Capacity;                     // +0x08
    unsigned long GrowthGranularity;            // +0x0c
    CArea         Root;                         // +0x10
    CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity);
    virtual ~CSimpleMemoryPool();
    void SetCapacity(unsigned long n) { Capacity = ((n + 0xf) / 16) * 16; }   // 0x00A5C5C0
};

CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity, unsigned long growthGranularity)
    : GrowthGranularity(((growthGranularity + 0xf) / 16) * 16)
{
    Root.Size = 0;
    Root.NextFreeArea = 0;
    FirstMemoryBlock = 0;
    Capacity = 0x400;
    if (capacity != 0)
        SetCapacity(capacity);
}