// CMemoryAllocator::LeaveSynchronisation() -- leave the allocator's critical section
// Leaf: ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2010, `push 13CAA70h ; call [0143FE10h] ; ret` (LeaveCriticalSection through the IAT).
// Identity (NF140, q4197 / q4198): Ego_r.pdb 0x00B45B90 / FableWin.pdb 0x030BFA50 ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ (protected static
//   __fastcall, no arguments), in the Ego_r run InitSynchronisation / UninitSynchronisation / EnterSynchronisation / LeaveSynchronisation
//   (0x00B45B60 / 0x00B45B70 / 0x00B45B80 / 0x00B45B90; retail 0x009F2000 / 0x009F2010 are the Enter / Leave pair called around every
//   PMemoryAllocator use by the inline Allocate / Deallocate of engine/CMemoryAllocator.h). Was the stand-in GFLeaveAllocatorLock (catalog) /
//   LeaveSharedLock (row).
// Globals: the allocator's CRITICAL_SECTION at 0x013CAA70 (no PDB data name found: the spelling of the landed CMemoryAllocator rows
//   0x009F2310 / 0x009F2B20).
#include "engine/CMemoryAllocator.h"
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock[24];   // 0x013CAA70 (CRITICAL_SECTION)

void __fastcall CMemoryAllocator::LeaveSynchronisation()
{
    LeaveCriticalSection(g_allocatorLock);
}
