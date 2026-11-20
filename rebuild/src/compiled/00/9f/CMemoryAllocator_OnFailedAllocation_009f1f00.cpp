// CMemoryAllocator::OnFailedAllocation -- empty in the release build (`ret`); descriptive of the retail body only, the name is the PDB's.
// Leaf: ?OnFailedAllocation@CMemoryAllocator@@QAEXXZ
// Naming (NF173, 2026-11-20): q11998: re-authored from the placeholder ?M@C009f1f00@@ onto the shared engine/CMemoryAllocator.h
//   (FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE declares it). Ego_r.pdb 0x00B46760 + FableWin.pdb 0x030BF3A0 public member; both landed referencers 0x00A65720 / 0x00A663D0 bind this spelling.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F1F00.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"

void CMemoryAllocator::OnFailedAllocation()
{
}
