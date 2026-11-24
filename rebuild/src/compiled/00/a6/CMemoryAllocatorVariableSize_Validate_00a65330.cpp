// CMemoryAllocatorVariableSize::Validate() -- publish the two usage counters to the statistics globals, then for every area of every chunk (+0xc1c, next at +0x20; areas from +0x30, next at +0) ask the global allocator (0x013CAA90, 0x009F1F40) to check the payload (header + the aligned header size, both re-read from the globals each time)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65330, length 96 bytes.
// Leaf: ?Validate@CMemoryAllocatorVariableSize@@UAEXXZ
// Callees: 0x009F1F40 (thiscall on the global allocator). Shape: the aligned header size recomputed from the two GLOBALS inside the loop (the call may change them), the chunk in edi, the area in esi, `ret`.
// Naming (NF161, 2026-11-20, q4196): the global allocator 0x013CAA90 is the protected static ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A
//   (Ego_r.pdb / FableWin.pdb / Ego_d.pdb), reached through the public inline ?Get@CMemoryAllocator@@SIPAV1@XZ (FableWin.pdb / Ego_d.pdb) of
//   the shared engine/CMemoryAllocator.h; the empty check 0x009F1F40 is ?InternalValidateArea@CMemoryAllocator@@QAEXPAX@Z (Ego_r.pdb / FableWin.pdb, public; Ego_r donor order
//   GetNoAllocations / SetCurrentTrackingID / GetCurrentTrackingID / InternalValidateArea / InternalValidate = retail 0x009F1F10 / 1F20 / 1F30 /
//   1F40 / 1F50 by body). Was ?ValidateBlock@CMemoryAllocator_00a65330@@QAEXPAX@Z on ?g_pGlobalAllocator_013caa90@@3PAUCMemoryAllocator_00a65330@@A.
struct SArea_00a65330 {
    SArea_00a65330* m_pNext;                       // +0x00
};
struct SVarChunk_00a65330 {
    unsigned char       _pad00[0x20];
    SVarChunk_00a65330* m_pNext;                   // +0x20
    unsigned char       _pad24[0xc];
    SArea_00a65330*     m_pFirstArea;              // +0x30
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_INTERNALVALIDATEAREA
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"   // NF161 (q4196): was the stand-in CMemoryAllocator_00a65330 + extern g_pGlobalAllocator_013caa90

class CMemoryAllocatorVariableSize { public:
    unsigned char _pad00[0xc10];
    unsigned long       m_usage;                   // +0xc14: the header size
    unsigned long       m_alignmentShift;          // +0xc18
    SVarChunk_00a65330* m_pFirstChunk;             // +0xc1c
    virtual void Validate();
};

void CMemoryAllocatorVariableSize::Validate()
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    for (SVarChunk_00a65330* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) {
        for (SArea_00a65330* area = chunk->m_pFirstArea; area; area = area->m_pNext)
            CMemoryAllocator::Get()->InternalValidateArea((char*)area + (((g_LastAllocatorUsage_013d28d4 + 0x1f) >> g_LastAllocatorChunkCounter_013d28d8) << g_LastAllocatorChunkCounter_013d28d8));
    }
}