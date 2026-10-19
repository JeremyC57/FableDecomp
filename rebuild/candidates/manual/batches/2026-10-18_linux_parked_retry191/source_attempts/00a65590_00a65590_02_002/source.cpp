// CMemoryAreaChunk::ConsolidateAdjacentFreeAreas -- merge physically adjacent free areas and restart the size-ordered free-list scan.
// Retail 0x00A65590, 248 bytes. No generated/quarantine CMemoryArea or CMemoryAreaChunk header exists.
// Native PDB layouts reused from independently landed Init/RemoveAreaFromFreeList.
// Globals 0x013D28D4 alignment, 0x013D28D8 shift; calls 0x00A65060 unlink and 0x00A650F0 reinsert.
#include <stddef.h>
#include "rebuild_abi.h"
struct CMemoryAreaChunk;
struct CMemoryAllocatorVariableSize;

// Process-wide copies of the active allocator's alignment, refreshed on entry to the allocator's
// public methods (0x013D28D4 alignment, 0x013D28D8 alignment shift).
extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)

struct CMemoryArea {                            // PDB: 32 bytes
    CMemoryArea*      PNextArea;                // +0x00
    CMemoryArea*      PRightChild;              // +0x04
    CMemoryArea*      PPreviousArea;            // +0x08
    CMemoryArea*      PParentArea;              // +0x0c
    CMemoryArea*      PLeftChild;               // +0x10
    CMemoryAreaChunk* PParentChunk;             // +0x14
    long              DataSize;                 // +0x18
    unsigned char     Colour;                   // +0x1c
    unsigned char     ID;                       // +0x1d
    void Init(CMemoryAreaChunk* owner,long size) {
        PNextArea=0; PRightChild=0; PPreviousArea=0; PParentArea=0; PLeftChild=0;
        Colour=0; PParentChunk=owner; DataSize=size;
    }
};

struct CMemoryAreaChunk {                       // PDB: 52 bytes
    long              TotalDataSize;            // +0x00
    long              NoFreeAreas;              // +0x04
    long              NoAllocatedAreas;         // +0x08
    long              FreeDataSize;             // +0x0c
    long              LargestFreeAreaSize;      // +0x10
    long              SmallestFreeAreaSize;     // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
    bool              Permanent;                // +0x1c
    bool              DemandPaged;              // +0x1d
    CMemoryAreaChunk* PNextChunk;               // +0x20
    CMemoryAreaChunk* PPreviousChunk;           // +0x24
    CMemoryArea*      PFirstFreeArea;           // +0x28
    CMemoryArea*      PLastFreeArea;            // +0x2c
    CMemoryArea*      PFirstAllocatedArea;      // +0x30
    void ConsolidateAdjacentFreeAreas(CMemoryArea* area);
    void RemoveAreaFromFreeList(CMemoryArea* area); // 0x00A65060
    void InsertAreaIntoFreeList(CMemoryArea* area);   // 0x00A650F0
};


FABLE_STATIC_ASSERT(sizeof(CMemoryArea)==32);
FABLE_STATIC_ASSERT(offsetof(CMemoryArea,DataSize)==0x18);
FABLE_STATIC_ASSERT(sizeof(CMemoryAreaChunk)==52);
FABLE_STATIC_ASSERT(offsetof(CMemoryAreaChunk,PFirstFreeArea)==0x28);
void CMemoryAreaChunk::ConsolidateAdjacentFreeAreas(CMemoryArea* input) {
    CMemoryArea* current=PFirstFreeArea;
    CMemoryArea* area=input;
    while(current) {
        CMemoryArea* next=current->PNextArea;
        if((char*)area+area->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea))==(char*)current) {
            RemoveAreaFromFreeList(area);
            RemoveAreaFromFreeList(current);
            area->Init(this, area->DataSize+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea)));
            InsertAreaIntoFreeList(area);
            next=PFirstFreeArea;
        } else if((char*)current+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea))==(char*)area) {
            RemoveAreaFromFreeList(area);
            RemoveAreaFromFreeList(current);
            current->Init(this, area->DataSize+current->DataSize+ALIGNED_SIZE(sizeof(CMemoryArea)));
            InsertAreaIntoFreeList(current);
            area=current;
            next=PFirstFreeArea;
        }
        current=next;
    }
}
