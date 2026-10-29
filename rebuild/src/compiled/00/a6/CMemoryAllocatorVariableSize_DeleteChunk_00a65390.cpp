// CMemoryAllocatorVariableSize::DeleteChunk(CMemoryAreaChunk* chunk) -- take every free area of the chunk out of the size tree, unlink the chunk from the allocator's chunk list and give its pages back to the OS (unless demand paged), sampling GlobalMemoryStatus before and after
// Leaf: DeleteChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65390, 126 bytes, `ret 4` (PDB ?DeleteChunk@CMemoryAllocatorVariableSize@@IAEXPAVCMemoryAreaChunk@1@@Z).
// Callees: CMemoryAllocatorVariableSize::RemoveFromTree(CMemoryArea*) @ 0x00A64EF0; KERNEL32 GlobalMemoryStatus
//   (IAT 0x0143FE20, twice through one register) and VirtualFree (IAT 0x0143FD40, MEM_RELEASE 0x8000).
//   Callers: 0x00A656C9, 0x00A65A94.
// Label: the manifest's `CMemoryAllocatorVariableSize_Deallocate` is descriptive; the PDB has DeleteChunk with this
//   signature and the body walks/unlinks a chunk, not an area.
// Layout: Ego_r.pdb CMemoryAllocatorVariableSize (PFirstChunk +0xc1c after the 0xc1c-byte CMemoryAllocatorBase);
//   CMemoryAreaChunk (52 bytes): DemandPaged +0x1d, PNextChunk +0x20, PPreviousChunk +0x24, PFirstFreeArea +0x28;
//   CMemoryArea: PNextArea +0. No engine header for the variable-size allocator: it derives from engine/CMemoryAllocatorBase.h.
// Shape: the MEMORYSTATUS is a 0x20-byte frame local whose figures only feed debug statistics, so release keeps the
//   two calls; `if (!chunk->IsDemandPaged()) VirtualFree(chunk, 0, MEM_RELEASE)` between them.
#include "engine/CMemoryAllocatorBase.h"

struct MEMORYSTATUS {
    unsigned long dwLength;
    unsigned long dwMemoryLoad;
    unsigned long dwTotalPhys;
    unsigned long dwAvailPhys;
    unsigned long dwTotalPageFile;
    unsigned long dwAvailPageFile;
    unsigned long dwTotalVirtual;
    unsigned long dwAvailVirtual;
};
extern "C" __declspec(dllimport) void __stdcall GlobalMemoryStatus(MEMORYSTATUS* status);
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {
public:
    class CMemoryAreaChunk;
    class CMemoryArea {
    public:
        CMemoryArea*      PNextArea;                    // +0x00
        CMemoryArea*      PRightChild;                  // +0x04
        CMemoryArea*      PPreviousArea;                // +0x08
        CMemoryArea*      PParentArea;                  // +0x0c
        CMemoryArea*      PLeftChild;                   // +0x10
        CMemoryAreaChunk* PParentChunk;                 // +0x14
        long              DataSize;                     // +0x18
        unsigned char     Colour;                       // +0x1c
        unsigned char     ID;                           // +0x1d
    };
    class CMemoryAreaChunk {
    public:
        long              TotalDataSize;                // +0x00
        long              NoFreeAreas;                  // +0x04
        long              NoAllocatedAreas;             // +0x08
        long              FreeDataSize;                 // +0x0c
        long              LargestFreeAreaSize;          // +0x10
        long              SmallestFreeAreaSize;         // +0x14
        CMemoryAllocatorVariableSize* PParentAllocator; // +0x18
        bool              Permanent;                    // +0x1c
        bool              DemandPaged;                  // +0x1d
        CMemoryAreaChunk* PNextChunk;                   // +0x20
        CMemoryAreaChunk* PPreviousChunk;               // +0x24
        CMemoryArea*      PFirstFreeArea;               // +0x28
        CMemoryArea*      PLastFreeArea;                // +0x2c
        CMemoryArea*      PFirstAllocatedArea;          // +0x30
        bool IsDemandPaged() const { return DemandPaged; }
    };

    CMemoryAreaChunk* PFirstChunk;                      // +0xc1c
    long              MinChunkSize;                     // +0xc20
    long              NoFreesSinceConsolidation;        // +0xc24
    unsigned long     Flags;                            // +0xc28
    unsigned long     InternalFlags;                    // +0xc2c
    unsigned long     ExtraMemUsedByOS;                 // +0xc30
    CMemoryArea*      PTreeRoot;                        protected: // +0xc34

    void RemoveFromTree(CMemoryArea* area); public:             // 0x00A64EF0
    void DeleteChunk(CMemoryAreaChunk* chunk);
};

void CMemoryAllocatorVariableSize::DeleteChunk(CMemoryAreaChunk* chunk)
{
    for (CMemoryArea* area = chunk->PFirstFreeArea; area; area = area->PNextArea)
        RemoveFromTree(area);
    if (chunk->PNextChunk)
        chunk->PNextChunk->PPreviousChunk = chunk->PPreviousChunk;
    if (chunk->PPreviousChunk)
        chunk->PPreviousChunk->PNextChunk = chunk->PNextChunk;
    if (PFirstChunk == chunk)
        PFirstChunk = chunk->PNextChunk;
    MEMORYSTATUS status;
    GlobalMemoryStatus(&status);
    if (!chunk->IsDemandPaged())
        VirtualFree(chunk, 0, 0x8000);
    GlobalMemoryStatus(&status);
}