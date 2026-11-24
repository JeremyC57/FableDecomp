#pragma optimize("s", on)
// CCountedPointer<const CFontBank>::Reset(const CFontBank* data) -- release (ReleaseData 0x0042F948), take data and give it a fresh count block (1, DeleteData, Data)
// Leaf: ?Reset@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042FC13, 59 bytes, thiscall `ret 4`. Callee: 0x0042F948.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?Reset@?$CCountedPointer@$$CBVCFontBank@@@@QAEXPBVCFontBank@@@Z; DeleteData<const CFontBank> 0x0042F864 stored in the count block.
// Was ?Reset@?$CCountedPointer@VCPolymorphicData_0042FC13@@@@QAEXPAVCPolymorphicData_0042FC13@@@Z (local model); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
// DeleteData<T> (address stored) is declared only (NF222: FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY, as the row declared it at HEAD): with T only
//   declared here the header's inline body would emit a pick-any plain-delete copy unlike the retail virtual-delete owner.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<const CFontBank>::Reset(const CFontBank*);
