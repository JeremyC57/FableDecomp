// Grade: DIFFER(167v169) -- the five zero stores use a fresh `xor ecx,ecx` before the shift where retail holds the zero in callee-saved ebx (`push ebx`) across the shift; everything else in order
// CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize(const char* name, unsigned long chunkSize, unsigned long areaSize, unsigned long alignment, unsigned long flags, unsigned long internalFlags) -- fixed-size pool allocator: no chunks yet, area size rounded up to the alignment (with the 4-byte area header), header size and areas per chunk derived from the chunk size
// Leaf: ??0CMemoryAllocatorFixedSize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65FF0, length 169 bytes (`ret 18h`), vtable 0x0129DB64.
// Callees: CMemoryAllocatorBase::CMemoryAllocatorBase(unsigned long alignment) @ 0x00A65AD0.
// Layout: engine/CMemoryAllocatorFixedSize.h offsets (they agree with retail: PFirstChunk +0xC1C .. NoEmptyChunks +0xC48), modelled
// locally with the same names because the generated header's `void* __vftable` and padded base cannot carry the base ctor and vtable;
// CMemoryAllocatorBase (Ego_r.pdb): PerformanceStats +0x04 (0xC10 bytes), Alignment +0xC14, AlignmentShift +0xC18.
// Shape: the name argument is unused; the chunk header size uses the base's alignment shift (`(0x1C + Alignment - 1) >> shift << shift`),
// the area size the alignment argument; the zero members and derived sizes are body assignments scheduled around the vtable store.
struct CMemoryAreaChunk;
struct CPerformanceStats { unsigned char _body[0xc10]; };
struct CMemoryAllocatorBase {
    CPerformanceStats PerformanceStats;         // +0x004
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
    CMemoryAllocatorBase(unsigned long alignment);   // 0x00A65AD0
    virtual ~CMemoryAllocatorBase();
    unsigned long AlignSize(unsigned long size) const { return ((size + Alignment - 1) >> AlignmentShift) << AlignmentShift; }
};
struct CMemoryAllocatorFixedSize : CMemoryAllocatorBase {
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    CMemoryAreaChunk* PLastChunk;               // +0xc20
    CMemoryAreaChunk* PCurrentAllocatorChunk;   // +0xc24
    unsigned long     AreaSize;                 // +0xc28
    unsigned long     StatAreaSize;             // +0xc2c
    unsigned long     ChunkHeaderSize;          // +0xc30
    unsigned long     NoAreasPerChunk;          // +0xc34
    unsigned long     ChunkSize;                // +0xc38
    unsigned long     Flags;                    // +0xc3c
    unsigned long     InternalFlags;            // +0xc40
    unsigned long     ExtraMemUsedByOS;         // +0xc44
    long              NoEmptyChunks;            // +0xc48
    CMemoryAllocatorFixedSize(const char* name, unsigned long chunkSize, unsigned long areaSize, unsigned long alignment,
                              unsigned long flags, unsigned long internalFlags);
    virtual ~CMemoryAllocatorFixedSize();
};

CMemoryAllocatorFixedSize::CMemoryAllocatorFixedSize(const char* name, unsigned long chunkSize, unsigned long areaSize,
                                                     unsigned long alignment, unsigned long flags, unsigned long internalFlags)
    : CMemoryAllocatorBase(alignment)
{
    AreaSize = (areaSize + 4 + alignment - 1) / alignment * alignment - 4;
    PCurrentAllocatorChunk = 0;
    ExtraMemUsedByOS = 0;
    NoEmptyChunks = 0;
    PFirstChunk = 0;
    PLastChunk = 0;
    ChunkHeaderSize = AlignSize(0x1c) - 4;
    StatAreaSize = AreaSize + 4;
    NoAreasPerChunk = (chunkSize - ChunkHeaderSize) / StatAreaSize;
    ChunkSize = NoAreasPerChunk * StatAreaSize + ChunkHeaderSize;
    Flags = flags;
    InternalFlags = internalFlags;
}
