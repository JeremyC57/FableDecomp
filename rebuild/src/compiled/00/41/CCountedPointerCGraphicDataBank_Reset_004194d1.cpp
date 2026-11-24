#pragma optimize("s", on)
// CCountedPointer<CGraphicDataBank>::Reset(CGraphicDataBank* data) -- release (ReleaseData 0x00419108), take data and give it a fresh count block (1, DeleteData, Data)
// Leaf: ?Reset@?$CCountedPointer@VCGraphicDataBank@@@@QAEXPAVCGraphicDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004194D1, 59 bytes, thiscall `ret 4`. Callee: 0x00419108.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?Reset@?$CCountedPointer@VCGraphicDataBank@@@@QAEXPAVCGraphicDataBank@@@Z; DeleteData<CGraphicDataBank> 0x00419036 stored in the count block (also stored by CGraphicBankManager::OpenGraphicBank 0x009F83D0).
// Was ?Reset@?$CCountedPointer@VCPolymorphicData_004194D1@@@@QAEXPAVCPolymorphicData_004194D1@@@Z (local model); ported onto engine/CCountedPointer.h (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY: the callee is out of line in this size TU).
// DeleteData<T> (address stored) is declared only (NF222: FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY, as the row declared it at HEAD): with T only
//   declared here the header's inline body would emit a pick-any plain-delete copy unlike the retail virtual-delete owner.
void* __cdecl operator new(unsigned int size);             // 0x00BFEA1A
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
template void CCountedPointer<CGraphicDataBank>::Reset(CGraphicDataBank*);
