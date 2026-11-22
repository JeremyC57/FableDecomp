#pragma optimize("s", on)
// CCountedPointer<CMeshDataBank>::Reset(CMeshDataBank* data) -- release (ReleaseData 0x0042918C), take data and give it a fresh count block (1, DeleteData, Data)
// Leaf: ?Reset@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042FC86, 59 bytes, thiscall `ret 4`. Callee: 0x0042918C.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?Reset@?$CCountedPointer@VCMeshDataBank@@@@QAEXPAVCMeshDataBank@@@Z; DeleteData<CMeshDataBank> 0x00428AE7 stored in the count block (also stored by CMeshBankManager::OpenMeshBank 0x00A09F20).
// Was same leaf; was a local CCountedPointer model calling ~CCountedPointer<CMeshDataBank>; ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
// DeleteData<T> (address stored) is the header's inline pick-any body; T is only declared here, as in the landed Reset rows 0x0042FD04 / 0x0042FD6F.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CMeshDataBank;
template void CCountedPointer<CMeshDataBank>::Reset(CMeshDataBank*);
