// CMemoryAllocatorVariableSize::GetPTreeRoot() -- the root of the free-area tree (+0xC34)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A64470, 7 bytes, `ret`. Unreferenced out-of-line copy of the inline getter.
// Layout: CMemoryAllocatorVariableSize (Ego_r.pdb, 0xC38 bytes) = CMemoryAllocatorBase (0xC1C) + PFirstChunk +0xC1C,
//   MinChunkSize, NoFreesSinceConsolidation, Flags, InternalFlags, ExtraMemUsedByOS, PTreeRoot +0xC34. No generated
//   header exists for the class, so the base header is the first member's model.
#include "engine/CMemoryAllocatorBase.h"

struct CMemoryArea;
struct CMemoryAreaChunk;

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase { public:
    CMemoryAreaChunk* PFirstChunk;                // +0xC1C
    long              MinChunkSize;               // +0xC20
    long              NoFreesSinceConsolidation;  // +0xC24
    unsigned long     Flags;                      // +0xC28
    unsigned long     InternalFlags;              // +0xC2C
    unsigned long     ExtraMemUsedByOS;           // +0xC30
    CMemoryArea*      PTreeRoot;                  // +0xC34
    CMemoryArea* GetPTreeRoot();
};

CMemoryArea* CMemoryAllocatorVariableSize::GetPTreeRoot()
{
    return PTreeRoot;
}