// CMemoryAllocatorFixedSize::CreateChunk(void* mem) -- carve a chunk out of a raw heap block: align the header up to the allocator's alignment, clear it, then thread every area of the chunk onto the chunk's free list (each new area pushed at the head; area header = own index, next index, previous index, allocated flag; 0xFF ends a chain); returns the chunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A661D0, length 137 bytes. No callees.
// Layout (PDB): CMemoryAllocatorBase Alignment +0xc14, AlignmentShift +0xc18; CMemoryAllocatorFixedSize StatAreaSize +0xc2c (the area stride,
//   as in the landed GetAllocatedArea 0x00A66100), ChunkHeaderSize +0xc30, NoAreasPerChunk +0xc34; CMemoryAreaChunk (0x18) as in DeleteChunk.
// Shape: the header address is `(mem + Alignment - 1) >> shift << shift`; the area's four header bytes are stored next, previous, own index,
//   flag (in that source order); the head's own index is read into the next byte through an if/else, and ebx doubles as the zero register.
struct CMemoryArea {
    unsigned char Index;                           // +0 the area's index within its chunk
    unsigned char NextIndex;                       // +1 next area on the free/allocated chain (0xFF = none)
    unsigned char PrevIndex;                       // +2 previous area on the chain
    unsigned char Allocated;                       // +3
};
struct CMemoryAreaChunk {
    void* ThisChunkMem;
    long NoAllocatedAreas;
    CMemoryAreaChunk* PNextChunk;
    CMemoryAreaChunk* PPreviousChunk;
    CMemoryArea* PFirstFreeArea;
    CMemoryArea* PFirstAllocatedArea;
};
struct CMemoryAllocatorFixedSize {
    unsigned char _base[0xc14];                    // vptr + CPerformanceStats
    unsigned long Alignment;          // +0xc14
    unsigned long AlignmentShift;     // +0xc18
    CMemoryAreaChunk* PFirstChunk;
    CMemoryAreaChunk* PLastChunk;
    CMemoryAreaChunk* PCurrentAllocatorChunk;
    unsigned long AreaSize;           // +0xc28 (PFirstChunk +0xc1c, PLastChunk +0xc20, PCurrentAllocatorChunk +0xc24 above)
    unsigned long StatAreaSize;       // +0xc2c
    unsigned long ChunkHeaderSize;    // +0xc30
    unsigned long NoAreasPerChunk;    // +0xc34
    CMemoryAreaChunk* CreateChunk(void* mem);
};
CMemoryAreaChunk* CMemoryAllocatorFixedSize::CreateChunk(void* mem)
{
    CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)((((unsigned long)mem + Alignment - 1) >> AlignmentShift) << AlignmentShift);
    chunk->ThisChunkMem = mem;
    chunk->PNextChunk = 0;
    chunk->PPreviousChunk = 0;
    chunk->PFirstFreeArea = 0;
    chunk->PFirstAllocatedArea = 0;
    chunk->NoAllocatedAreas = 0;
    CMemoryArea* area = (CMemoryArea*)((unsigned char*)chunk + ChunkHeaderSize);
    for (unsigned long i = 0; i < NoAreasPerChunk; ++i) {
        area->NextIndex = 0xff;
        area->PrevIndex = 0xff;
        area->Index = (unsigned char)i;
        area->Allocated = 0;
        if (chunk->PFirstFreeArea == 0)
            area->NextIndex = 0xff;
        else
            area->NextIndex = chunk->PFirstFreeArea->Index;
        if (chunk->PFirstFreeArea)
            chunk->PFirstFreeArea->PrevIndex = (unsigned char)i;
        chunk->PFirstFreeArea = area;
        area = (CMemoryArea*)((unsigned char*)area + StatAreaSize);
    }
    return chunk;
}