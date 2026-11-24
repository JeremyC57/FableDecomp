#pragma optimize("s", on)
// CCountedPointer<CFontBank>::Reset(CFontBank* data) -- release (ReleaseData 0x004057A0), take data and give it a fresh count block (1, DeleteData, Data)
// Leaf: ?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041947A, 59 bytes, thiscall `ret 4`. Callee: 0x004057A0.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?Reset@?$CCountedPointer@VCFontBank@@@@QAEXPAVCFontBank@@@Z; DeleteData<CFontBank> 0x00419028 (virtual delete) stored in the count block; callers 0x00A5F1C0 CStreamingFontDataBankEntry::AcquireFont and 0x00A634F0 CFontDataBankEntry::AcquireFont; Ego_r.pdb also keeps ?Reset@?$CCountedPointer@VCFontBank@@@@ out of line.
// Was ?Reset@?$CCountedPointer@VCPolymorphicData_0041947A@@@@QAEXPAVCPolymorphicData_0041947A@@@Z (local model); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
// DeleteData<T> (address stored) is declared only (NF222: FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY, as the row declared it at HEAD): with T only
//   declared here the header's inline body would emit a pick-any plain-delete copy unlike the retail virtual-delete owner.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CFontBank;
template void CCountedPointer<CFontBank>::Reset(CFontBank*);
