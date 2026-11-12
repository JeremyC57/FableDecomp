// CCountedPointer::Reset(T* data) -- (CCountedPointer<CPlayerGui>) drop the current share (destroying the pointee on the last reference), then adopt data with a fresh count block
// Leaf: ?Reset@?$CCountedPointer@VCPlayerGui@@@@QAEXPAVCPlayerGui@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004195AF, 59 bytes, thiscall, `ret 4`. Stores DeleteData 0x00419091.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES; was CCountedPointer<CPolymorphicData_004195AF>::Reset, a stand-in with a local count-block model): FableWin.pdb
//   ?Reset@?$CCountedPointer@VCPlayerGui@@@@QAEXPAVCPlayerGui@@@Z; the body stores CCountedPointer<CPlayerGui>::DeleteData 0x00419091 (FAM10, row MATCH) as the count block's DeleteFunc.
// Shape: size-optimised TU (odd entry): ReleaseData<CPlayerGui> 0x0041915A is called out of line (FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY),
//   then Data = data and the count block for a non-null data (Data re-read through `this`).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CPlayerGui;
template void CCountedPointer<CPlayerGui>::Reset(CPlayerGui*);
