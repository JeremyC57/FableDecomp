// CMemoryAreaChunk::SetDemandPaged(bool demandPaged) -- store the chunk's demand-paged flag (bool DemandPaged +0x1D)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A64440, 10 bytes, `ret 4`. Unreferenced out-of-line copy of the inline setter.
// Layout: CMemoryAreaChunk is the 0x34-byte variable-size allocator chunk of Ego_r.pdb (TotalDataSize, NoFreeAreas,
//   NoAllocatedAreas, FreeDataSize, LargestFreeAreaSize, SmallestFreeAreaSize, PParentAllocator +0x18, Permanent +0x1C,
//   DemandPaged +0x1D, the chunk links +0x20/+0x24 and the area lists +0x28..+0x30). No generated header exists.
struct CMemoryArea;
class CMemoryAllocatorVariableSize;

struct CMemoryAreaChunk {
    long                          TotalDataSize;        // +0x00
    long                          NoFreeAreas;          // +0x04
    long                          NoAllocatedAreas;     // +0x08
    long                          FreeDataSize;         // +0x0C
    long                          LargestFreeAreaSize;  // +0x10
    long                          SmallestFreeAreaSize; // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;     // +0x18
    bool                          Permanent;            // +0x1C
    bool                          DemandPaged;          // +0x1D
    CMemoryAreaChunk*             PNextChunk;           // +0x20
    CMemoryAreaChunk*             PPreviousChunk;       // +0x24
    CMemoryArea*                  PFirstFreeArea;       // +0x28
    CMemoryArea*                  PLastFreeArea;        // +0x2C
    CMemoryArea*                  PFirstAllocatedArea;  // +0x30
    void SetDemandPaged(bool demandPaged);
};

void CMemoryAreaChunk::SetDemandPaged(bool demandPaged)
{
    DemandPaged = demandPaged;
}