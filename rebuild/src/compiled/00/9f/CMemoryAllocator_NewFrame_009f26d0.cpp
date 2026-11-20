// CMemoryAllocator::NewFrame() -- release build: only the (inlined) Enter/LeaveSynchronisation pair on the allocator lock remains
// Leaf: ?NewFrame@CMemoryAllocator@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F26D0, `push 13CAA70h ; call [0143FE0Ch] ; push 13CAA70h ; call [0143FE10h] ; ret`.
// Identity (NF177, PRIORITY q12001 / q12097): Ego_r.pdb / FableWin.pdb ?NewFrame@CMemoryAllocator@@QAEXXZ (public); Ego_r donor order
//   ClearPerformanceStats / NewFrame (0x00B46220 / 0x00B46240) = retail 0x009F26B0 / 0x009F26D0; the only landed referencer 0x00416047
//   already binds this spelling (engine/CMemoryAllocator.h FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME). Re-authored under it: the landed row
//   was the free __fastcall stand-in ClearPerformanceStats(CMemoryAllocator*) (catalog duplicate of 0x009F26B0) on g_perfStatsObject.
// Body: the static EnterSynchronisation / LeaveSynchronisation (out of line at 0x009F2000 / 0x009F2010, landed) are expanded here, so the
//   row spells their bodies: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the allocator's
//   CRITICAL_SECTION 0x013CAA70 (no PDB data name found: the spelling of the landed rows 0x009F2000 / 0x009F2310).
#define FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME
#include "engine/CMemoryAllocator.h"
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock[24];   // 0x013CAA70 (CRITICAL_SECTION)

void CMemoryAllocator::NewFrame()
{
    EnterCriticalSection(g_allocatorLock);
    LeaveCriticalSection(g_allocatorLock);
}
