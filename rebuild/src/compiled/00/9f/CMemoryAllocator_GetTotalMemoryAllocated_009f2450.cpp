// CMemoryAllocator::GetTotalMemoryAllocated() -- under the allocator lock, sum the memory allocated by every sub-allocator of the allocator-info table (0x01396868, 28-byte records, PAllocator at +0x18; count NoAllocators +0x410) through virtual slot 7
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2450, length 77 bytes.
// Leaf: ?GetTotalMemoryAllocated@CMemoryAllocator@@QAEKXZ
// Identity (NF42, round 5): Ego_r.pdb 0x00B45FD0 / FableWin.pdb 0x030BDE80 ?GetTotalMemoryAllocated@CMemoryAllocator@@QAEKXZ. The
// sub-allocator call is CMemoryAllocatorBase vtable slot 7 (+0x1c), which the PDB interface (engine/CMemoryAllocatorBase.h) names
// GetTotalMemoryAllocated; GetHeaderOverhead is slot 12 and belongs to the sibling row 0x009F2610. The landed caller
// CDefinitionManager::SaveBinaryDefinitions (0x009AF7A0) already binds this symbol here. Was spelled GetHeaderOverhead on a local
// CMemoryAllocator model; now on the shared headers.
// Callees: EnterCriticalSection / LeaveCriticalSection on the allocator lock @ 0x013CAA70 (IAT 0x0143FE0C / 0x0143FE10).
// Shape: the sum in ebp, the table pointer stepped by 0x1c in edi, the count (+0x410) re-read after every call, `ret`.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_PASS23
#include "engine/CMemoryAllocatorBase.h"
#include "engine/CMemoryAllocator.h"

extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock[24];   // 0x013CAA70: the allocator critical section (descriptive; the 0x009F2310 / 0x009F2610 spelling)

unsigned long CMemoryAllocator::GetTotalMemoryAllocated()
{
    unsigned long total = 0;
    EnterCriticalSection(g_allocatorLock);
    for (unsigned long i = 0; i < NoAllocators; ++i)
        total += g_AllocatorInfo[i].PAllocator->GetTotalMemoryAllocated();
    LeaveCriticalSection(g_allocatorLock);
    return total;
}
