// CMemoryAllocator::SaveStackMemoryTrackerLog -- empty in the release build (`ret` ret 4); descriptive of the retail body only, the name is the PDB's.
// Leaf: ?SaveStackMemoryTrackerLog@CMemoryAllocator@@QAEXPBD@Z
// Naming (NF173, 2026-11-20): q12000: re-authored from the placeholder ?M@C009f2080@@ onto the shared engine/CMemoryAllocator.h
//   (FABLE_RETAIL_CMEMORYALLOCATOR_SAVESTACKMEMORYTRACKERLOG declares it). Ego_r.pdb + FableWin.pdb public member (Ego_r donor order ... IsAllocatorFixedSize / SaveStackMemoryTrackerLog = retail 0x009F2060 / 0x009F2080); the only landed referencer 0x00415E85 binds this spelling.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F2080.
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVESTACKMEMORYTRACKERLOG
#include "engine/CMemoryAllocator.h"

void CMemoryAllocator::SaveStackMemoryTrackerLog(const char* file_name)
{
}
