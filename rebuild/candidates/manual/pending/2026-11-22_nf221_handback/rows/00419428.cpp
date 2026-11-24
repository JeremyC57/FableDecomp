#pragma optimize("s", on)
// CCountedPointer<CLUA>::Reset(CLUA* data) -- release (ReleaseData 0x004190B6), take data and give it a fresh count block (1, DeleteData, Data)
// Leaf: ?Reset@?$CCountedPointer@VCLUA@@@@QAEXPAVCLUA@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419428, 59 bytes, thiscall `ret 4`. Callee: 0x004190B6.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?Reset@?$CCountedPointer@VCLUA@@@@QAEXPAVCLUA@@@Z; DeleteData<CLUA> 0x0041901D stored in the count block.
// Was ?Reset@?$CCountedPointer@VCPolymorphicData_00419428@@@@QAEXPAVCPolymorphicData_00419428@@@Z (local model); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
// DeleteData<T> (address stored) is the header's inline pick-any body; T is only declared here, as in the landed Reset rows 0x0042FD04 / 0x0042FD6F.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CLUA;
template void CCountedPointer<CLUA>::Reset(CLUA*);
