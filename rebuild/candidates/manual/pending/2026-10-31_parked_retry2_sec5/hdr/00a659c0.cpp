// CMemoryAllocatorVariableSize::Allocate(unsigned long) -- allocate a block: try the existing chunks, else add a chunk and retry; returns the data start past the area header or 0
// Grade: DIFFER(140v140) -- moved onto engine/CMemoryAllocatorBase.h + the landed 0x00A656E0 model with the landed
//   callee names (bytes as parked): after the global stores retail reloads Alignment into edx before AlignmentShift into
//   ecx; every Align spelling (size+A-1, A+size-1, named shift, named rounded sum) loads ecx first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A659C0, 140 bytes, thiscall virtual (vtable 0x0129DB1C slot 0), `ret 4`.
// Callees: CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(unsigned long) @ 0x00A656E0 (landed; inlined at the
//   first call, called for the retry), GetTreeAreaWithSizeGreaterThanOrEqualTo(long) const @ 0x00A64C10 (landed name; the
//   parked file called it AllocateMemoryFromExistingChunks and 0x00A656E0 "AllocateArea"), CMemoryAreaChunk::AllocAreaFromArea
//   @ 0x00A65480, NewChunk(long) @ 0x00A65720 (reloc_bindings ?NewChunk@CMemoryAllocatorVariableSize@@QAEPAUCMemoryAreaChunk@@J@Z).
// Layout: engine/CMemoryAllocatorBase.h (Alignment +0xc14, AlignmentShift +0xc18) and the landed 0x00A656E0 model of the
//   Ego_r.pdb CMemoryAllocatorVariableSize (nested CMemoryArea 32 bytes, CMemoryAreaChunk).
// Globals: 0x013D28D4 / 0x013D28D8 (g_CurrentAlignment / g_CurrentAlignmentShift, the spelling the 0x00A65260 caller binds):
//   the active allocator's alignment, used by the header-size rounding of the data start.
// Shape: the members' Align inline (landed `(size + Alignment - 1) >> AlignmentShift << AlignmentShift`) inside an in-class
//   AllocateMemoryFromExistingChunks: VC7.1 inlines the first call and keeps the retry out of line, as retail.
#include "engine/CMemoryAllocatorBase.h"

extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

struct CMemoryAreaChunk;

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {
public:
    class CMemoryArea;
    class CMemoryAreaChunk {
    public:
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);   // 0x00A65480
    };
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
        void* GetDataStart()
        { return (char*)this + (((sizeof(CMemoryArea) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift); }
    };

    ::CMemoryAreaChunk* PFirstChunk;                    // +0xc1c
    long              MinChunkSize;                     // +0xc20
    long              NoFreesSinceConsolidation;        // +0xc24
    unsigned long     Flags;                            // +0xc28
    unsigned long     InternalFlags;                    // +0xc2c
    unsigned long     ExtraMemUsedByOS;                 // +0xc30
    CMemoryArea*      PTreeRoot;                        // +0xc34

    unsigned long Align(unsigned long size) const { return (size + Alignment - 1) >> AlignmentShift << AlignmentShift; }
    protected: CMemoryArea* GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const; public:   // 0x00A64C10
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size)                          // 0x00A656E0
    {
        size = Align(size);
        CMemoryArea* area = GetTreeAreaWithSizeGreaterThanOrEqualTo(size);
        if (area)
            return area->PParentChunk->AllocAreaFromArea(area, size);
        return 0;
    }
    ::CMemoryAreaChunk* NewChunk(long size);                                                   // 0x00A65720
    void* Allocate(unsigned long size);
};

void* CMemoryAllocatorVariableSize::Allocate(unsigned long size)
{
    g_CurrentAlignmentShift = AlignmentShift;
    g_CurrentAlignment = Alignment;
    CMemoryArea* area = AllocateMemoryFromExistingChunks(size);
    if (area == 0) {
        if (NewChunk(size) == 0)
            return 0;
        area = AllocateMemoryFromExistingChunks(size);
    }
    if (area)
        return area->GetDataStart();
    return 0;
}
