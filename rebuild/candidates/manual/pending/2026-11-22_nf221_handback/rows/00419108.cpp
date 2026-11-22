#pragma optimize("s",on)
// CCountedPointer<CGraphicDataBank>::ReleaseData() -- release the shared count (run the deleter and free the block when it drops to zero), then zero Data and Info
// Leaf: ?ReleaseData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00419108, 44 bytes, thiscall `ret`. Callees: the count block's DeleteFunc (indirect), operator delete 0x00BFE9BC.
// Identity (NF221, family q12226 / q12402): FableWin.pdb ?ReleaseData@?$CCountedPointer@VCGraphicDataBank@@@@QAEXXZ; destructor thunk 0x004194B5 (`jmp 0x00419108`; landed caller 0x0041AFA0 binds ??1 there), as the
//   CComponent pair 0x004291DE / 0x00429C53 (`~CCountedPointer() { ReleaseData(); }`); called by ShareData<CGraphicDataBank> 0x00419134 and Reset<CGraphicDataBank> 0x004194D1 (both call ReleaseData in the header body).
//   Was landed as ~CCountedPointer<CGraphicDataBank>.
// Model: engine/CCountedPointer.h opt-in FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (Data zeroed before Info). This size-optimised TU keeps ReleaseData out of line by itself.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#include "engine/CCountedPointer.h"
class CGraphicDataBank;
template void CCountedPointer<CGraphicDataBank>::ReleaseData();
