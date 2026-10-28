// CMemoryAllocatorVariableSize::GetAllocatedArea(long index) -- publish the alignment pair to the area-header globals, then walk every chunk's allocated-area list counting areas; the data pointer (area + the aligned header size) of the index-th area, or null
// Leaf: ?GetAllocatedArea@CMemoryAllocatorVariableSize@@UAEPAXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A652D0, 96 bytes, `ret 4`. Name: Ego_r.pdb ?GetAllocatedArea@CMemoryAllocatorVariableSize@@UAEPAXJ@Z;
//   the catalog label agrees (twin of the landed CMemoryAllocatorFixedSize::GetAllocatedArea 0x00A66100).
// Layout: Ego_r.pdb CMemoryAllocatorBase Alignment +0xC14, AlignmentShift +0xC18; CMemoryAllocatorVariableSize PFirstChunk
//   +0xC1C; CMemoryAreaChunk PNextChunk +0x20, PFirstAllocatedArea +0x30; CMemoryArea PNextArea +0.
// Globals: 0x013D28D8 / 0x013D28D4 take AlignmentShift / Alignment (spelled as the landed siblings 0x00A651E0, 0x00A64560
//   spell them; the real static names are not in the PDB dumps).
// Shape: the members stored straight to the globals and the aligned header size read back FROM THE GLOBALS on the found
//   exit (VC forwards the stored registers: Alignment stays in ebx, the index is loaded lazily into edi); the local-copy form
//   of the landed GetAllocationSizeIncludingHeader swaps ebx/edi here. The count is compared with the index at the top of
//   the inner walk.
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

class CMemoryAllocatorVariableSize {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
        CMemoryArea* PNextArea;                         // +0x00
    };
    class CMemoryAreaChunk {
    public:
        unsigned char     _pad00[0x20];
        CMemoryAreaChunk* PNextChunk;                   // +0x20
        unsigned char     _pad24[0xc];
        CMemoryArea*      PFirstAllocatedArea;          // +0x30
    };
    virtual void* GetAllocatedArea(long index);
    unsigned char     _pad04[0xc10];
    unsigned long     Alignment;                        // +0xC14
    unsigned long     AlignmentShift;                   // +0xC18
    CMemoryAreaChunk* PFirstChunk;                      // +0xC1C
};

void* CMemoryAllocatorVariableSize::GetAllocatedArea(long index)
{
    g_LastAllocatorChunkCounter_013d28d8 = AlignmentShift;
    g_LastAllocatorUsage_013d28d4 = Alignment;
    long count = 0;
    for (CMemoryAreaChunk* chunk = PFirstChunk; chunk; chunk = chunk->PNextChunk) {
        for (CMemoryArea* area = chunk->PFirstAllocatedArea; area; area = area->PNextArea) {
            if (count == index)
                return (char*)area + (((g_LastAllocatorUsage_013d28d4 + 0x1f) >> g_LastAllocatorChunkCounter_013d28d8) << g_LastAllocatorChunkCounter_013d28d8);
            ++count;
        }
    }
    return 0;
}